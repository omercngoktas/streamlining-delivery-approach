from functools import partial
from selenium_fetcher import fetch_duration_and_distance
from get_location_details import get_depot, get_stores
import pandas as pd
from time import sleep
import multiprocessing
import threading
from selenium import webdriver

# # def main():
# #     stores = get_stores()
# #     depot = get_depot()
# #     # create a dataframe which contains the store details and duration and distance from this store to next store
# #     # also contains the detail of this next store
# #     output = pd.DataFrame(columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    
# #     # Create a pool of processes
# #     pool = multiprocessing.Pool(processes=multiprocessing.cpu_count())  # Use number of CPU cores for parallel processing
    
# #     # iterate through all the stores
# #     for i in range(len(stores)):
# #         for j in range(len(stores)):
# #             if i != j:
# #                 travel_time, distance = fetch_duration_and_distance(stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude'])
# #                 # output = output.append({'current_id': stores.iloc[i]['store_id'], 'current_latitude': stores.iloc[i]['latitude'], 'current_longitude': stores.iloc[i]['longitude'], 'duration': travel_time, 'distance': distance, 'next_id': stores.iloc[j]['store_id'], 'next_latitude': stores.iloc[j]['latitude'], 'next_longitude': stores.iloc[j]['longitude']}, ignore_index=True)
# #                 new_output = pd.DataFrame([[stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], travel_time, distance, stores.iloc[j]['store_id'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
# #                 # adding new row to the output dataframe with using concat
# #                 output = pd.concat([output, new_output], ignore_index=True)
# #                 output.to_csv('./output.csv', index=False)
                
                
# # if __name__ == "__main__":
# #     main()

import threading
import pandas as pd

# Initialize semaphore with desired concurrency limit
semaphore = threading.Semaphore(15)  # Limit to 5 concurrent threads

def main():
    stores = get_stores()
    output = pd.DataFrame(columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    
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
    
    # Process the fetched data
    for result in results:
        print(result)
    
    df = pd.DataFrame(results, columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    df.to_csv('output.csv', index=False)


def process_data(i, j, stores, results):
    # Acquire semaphore to limit concurrency
    semaphore.acquire()
    try:
        # Fetch duration and distance for the pair of coordinates
        travel_time, distance = fetch_duration_and_distance(stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude'])
        # new_output = pd.DataFrame([[stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], travel_time, distance, stores.iloc[j]['store_id'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
        # # Append fetched data to the results list
        # output = pd.concat([output, new_output], ignore_index=True)
        results.append([stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], travel_time, distance, stores.iloc[j]['store_id'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude']])
        
        
    finally:
        # Release semaphore after processing
        semaphore.release()

if __name__ == "__main__":
    main()
