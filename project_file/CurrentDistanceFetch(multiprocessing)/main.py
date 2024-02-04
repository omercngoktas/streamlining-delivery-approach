from selenium_fetcher import fetch_duration_and_distance
from get_location_details import get_depot, get_stores
from find_missing import find_missing_results
import pandas as pd
import multiprocessing
import time
import os

# def main():
#     start_time = time.time()
#     stores = get_stores()
#     depot = get_depot()
    
#     # Determine the number of available CPU cores
#     num_processes = multiprocessing.cpu_count()
#     manager = multiprocessing.Manager()
    
#     # Create a process pool with the determined number of processes
#     pool = multiprocessing.Pool(processes=num_processes)
    
#     # List to store fetched data
#     results = []
    
#     # Create processes for each pair of coordinates
#     for i in range(len(stores)):
#         for j in range(len(stores)):
#             if i != j:
#                 pool.apply_async(process_data, args=(i, j, stores, results))
    
#     # Close the pool and wait for all processes to complete
#     pool.close()
#     pool.join()
    
#     # Sort and save the results
#     results.sort(key=lambda x: x[0])
#     df = pd.DataFrame(results, columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
#     df.to_csv('output.csv', index=False)
    
#     # Find missing results
#     find_missing_results(stores, results)
    
#     print("--- %s seconds ---" % (time.time() - start_time))


# def process_data(i, j, stores, results):
#     # Fetch duration and distance for the pair of coordinates
#     travel_time, distance = fetch_duration_and_distance(stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude'])
    
#     # Append the fetched data to the results list
#     results.append([stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], travel_time, distance, stores.iloc[j]['store_id'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude']])
#     print('Processed: ', i, j)
    


file_lock = multiprocessing.Lock()

def process_state(i, j, stores):
    travel_time, distance = fetch_duration_and_distance(stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude'])
    output_df = pd.DataFrame([[stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], travel_time, distance, stores.iloc[j]['store_id'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    
    with file_lock:
        output_df.to_csv("output.csv", mode="a", index=False, header=not os.path.exists("output.csv"))

def main():
    start_time = time.time()
    stores = get_stores()
    depot = get_depot()
    output_df = pd.DataFrame(columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    output_df.to_csv("output.csv", index=False)
    
    cpu_count = multiprocessing.cpu_count()
    print("Number of CPU cores: ", cpu_count)
    pool = multiprocessing.Pool(processes=cpu_count)
    
    # Create a list of argument tuples for all combinations of i and j
    args_list = [(i, j, stores) for i in range(len(stores)) for j in range(len(stores)) if i != j]
    
    # Use pool.starmap() to execute process_state() with each set of arguments
    pool.starmap(process_state, args_list)
    
    pool.close()
    pool.join()
    
    get_output = pd.read_csv("output.csv")
    results = get_output.values.tolist()
    find_missing_results(stores, results)
    
    print('All processes completed in ', time.time() - start_time, ' seconds')

if __name__ == "__main__":
    main()
