from selenium_fetcher import fetch_duration_and_distance
from get_location_details import get_depot, get_stores
from find_missing import find_missing_results
import pandas as pd
import threading
import time

# Initialize semaphore with desired concurrency limit
semaphore = threading.Semaphore(30)  # Limit to 5 concurrent threads

def main():
    start_time = time.time()
    stores = get_stores()
    depot = get_depot()
    # List to store fetched data
    results = []
    # Create threads for each pair of coordinates
    threads = []
    for i in range(len(stores)):
        for j in range(len(stores)):
            if i != j:
                thread = threading.Thread(target=lambda i=i, j=j: process_data(i, j, stores, results))
                threads.append(thread)
                thread.start()
    
    # Wait for all threads to complete
    for thread in threads:
        thread.join()
    
    results.sort(key=lambda x: x[0])
    df = pd.DataFrame(results, columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    df.to_csv('output.csv', index=False)
    
    find_missing_results(stores, results)
    
    print("--- %s seconds ---" % (time.time() - start_time))


def process_data(i, j, stores, results):
    # Acquire semaphore to limit concurrency
    semaphore.acquire()
    try:
        # Fetch duration and distance for the pair of coordinates
        travel_time, distance = fetch_duration_and_distance(stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude'])
        results.append([stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], travel_time, distance, stores.iloc[j]['store_id'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude']])
        print('Processed: ', i, j)
        
    finally:
        # Release semaphore after processing
        semaphore.release()

if __name__ == "__main__":
    main()
