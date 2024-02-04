from selenium_fetcher import fetch_duration_and_distance
from get_location_details import get_depot, get_stores
from find_missing import find_missing_results
from get_todays_shipments import get_days_shipment_output
import pandas as pd
import multiprocessing
import time
import os
import datetime

file_lock = multiprocessing.Lock()

def process_state_stores(current_store, next_store):
    travel_time, distance = fetch_duration_and_distance(current_store['latitude'], current_store['longitude'], next_store['latitude'], next_store['longitude'])
    output_df = pd.DataFrame([[current_store['store_id'], current_store['latitude'], current_store['longitude'], travel_time, distance, next_store['store_id'], next_store['latitude'], next_store['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    # if any element of the output_df is None, then skip the write to the file
    if output_df.isnull().values.any():
        print("Skipping write to file")
        return
    
    with file_lock:
        print("Output: ", output_df.values.tolist())
        output_df.to_csv("stores_output.csv", mode="a", index=False, header=not os.path.exists("stores_output.csv"))

def arrange_stores(stores, stores_with_shipment):
    arranged_stores = []
    # if any of the stores in stores is in stores_with_shipment, then add it to arranged_stores
    for index, row in stores.iterrows():
        if row["store_id"] in stores_with_shipment.values:
            arranged_stores.append([row["store_id"], row["latitude"], row["longitude"]])
    
    return arranged_stores

def process_state_depot(depot, current_store):
    travel_time, distance = fetch_duration_and_distance(depot['latitude'], depot['longitude'], current_store['latitude'], current_store['longitude'])
    output_df = pd.DataFrame([[depot['depot_id'], depot['latitude'], depot['longitude'], travel_time, distance, current_store['store_id'], current_store['latitude'], current_store['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    # if any element of the output_df is None, then skip the write to the file
    if output_df.isnull().values.any():
        print("Skipping write to file")
        return
    
    with file_lock:
        print("Output: ", output_df.values.tolist())
        output_df.to_csv("depot_output.csv", mode="a", index=False, header=not os.path.exists("depot_output.csv"))

def create_pool_for_stores(cpu_count, stores, current_store):
    print("Creating pool for store ", current_store["store_id"])
    
    args_list = []
    
    for index, next_store in stores.iterrows():
        if next_store["store_id"] != current_store["store_id"]:
            args_list.append((current_store, next_store))
    
    pool = multiprocessing.Pool(processes=cpu_count)
    pool.starmap(process_state_stores, args_list)
    pool.close()
    pool.join()

def create_pool_for_depot(cpu_count, stores, depot):
    print("Creating pool for depot ", depot["depot_id"])
    args_list = []
    
    for index, next_store in stores.iterrows():
        args_list.append((depot, next_store))
    
    pool = multiprocessing.Pool(processes=cpu_count)
    pool.starmap(process_state_depot, args_list)
    pool.close()
    pool.join()

def main():
    today = datetime.date.today()
    start_time = time.time()
    get_days_shipment_output(today.day, today.month, today.year)
    
    depot_df = get_depot()
    stores = get_stores(file_path="../data/store/stores.csv")
    stores_with_shipment = get_stores("./stores_with_shipment.csv")
    
    arranged_stores = arrange_stores(stores, stores_with_shipment)
    stores_to_visit = pd.DataFrame(arranged_stores, columns=["store_id", "latitude", "longitude"])
    output_df = pd.DataFrame(columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    output_df.to_csv("stores_output.csv", index=False)
    output_df.to_csv("depot_output.csv", index=False)
    
    cpu_count = 24
    
    create_pool_for_stores(cpu_count, stores_to_visit, stores_to_visit.iloc[0])
    create_pool_for_depot(cpu_count, stores_to_visit, depot_df.iloc[0])
    
    # get_output = pd.read_csv("stores_output.csv")
    # results = get_output.values.tolist()
    # find_missing_results(stores_to_visit, results)
    
    # get_output = pd.read_csv("depot_output.csv")
    # depot_results = get_output.values.tolist()
    # find_missing_results(depot_df, depot_results)
    
    
    
    print('All processes completed in ', time.time() - start_time, ' seconds')

if __name__ == "__main__":
    main()
