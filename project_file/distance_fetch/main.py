from selenium_fetcher import fetch_duration_and_distance
from get_location_details import get_depot, get_stores
from find_missing import find_missing_results
import pandas as pd
import multiprocessing
import time
import os

file_lock = multiprocessing.Lock()

def process_state(i, j, stores):
    travel_time, distance = fetch_duration_and_distance(stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude'])
    output_df = pd.DataFrame([[stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], travel_time, distance, stores.iloc[j]['store_id'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    
    # if any element of the output_df is None, then skip the write to the file
    if output_df.isnull().values.any():
        print("Skipping write to file")
        return
    
    with file_lock:
        print("Output: ", output_df.values.tolist())
        output_df.to_csv("output.csv", mode="a", index=False, header=not os.path.exists("output.csv"))

def main():
    start_time = time.time()
    stores = get_stores()
    depot = get_depot()
    output_df = pd.DataFrame(columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    output_df.to_csv("output.csv", index=False)
    
    cpu_count = multiprocessing.cpu_count()
    print("Number of CPU cores: ", cpu_count)
    pool = multiprocessing.Pool(processes=24)
    
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
