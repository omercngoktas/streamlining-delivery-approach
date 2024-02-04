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

def process_state_stores(i, j, stores):
    travel_time, distance = fetch_duration_and_distance(stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude'])
    output_df = pd.DataFrame([[stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], travel_time, distance, stores.iloc[j]['store_id'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
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


def process_state_depot_stores(i, depot_df, stores):
    travel_time, distance = fetch_duration_and_distance(depot_df.iloc[0]['latitude'], depot_df.iloc[0]['longitude'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'])
    output_df = pd.DataFrame([[depot_df.iloc[0]['depot_id'], depot_df.iloc[0]['latitude'], depot_df.iloc[0]['longitude'], travel_time, distance, stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    # if any element of the output_df is None, then skip the write to the file
    if output_df.isnull().values.any():
        print("Skipping write to file")
        return
    
    with file_lock:
        print("Output: ", output_df.values.tolist())
        output_df.to_csv("depot_output.csv", mode="a", index=False, header=not os.path.exists("depot_output.csv"))

def main():
    today = datetime.date.today()

    start_time = time.time()
    get_days_shipment_output(today.day, today.month, today.year)
    stores = get_stores(file_path="../data/store/stores.csv")
    stores_with_shipment = get_stores("./stores_with_shipment.csv")
    arranged_stores = arrange_stores(stores, stores_with_shipment)
    stores_to_visit = pd.DataFrame(arranged_stores, columns=["store_id", "latitude", "longitude"])
    depot_df = get_depot()
    output_df = pd.DataFrame(columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    output_df.to_csv("stores_output.csv", index=False)
    output_df.to_csv("depot_output.csv", index=False)

    cpu_count = multiprocessing.cpu_count()
    print("Number of CPU cores: ", cpu_count)
    pool = multiprocessing.Pool(processes=24)

    # Create a list of argument tuples for all combinations of i and j
    args_list = [(i, j, stores_to_visit) for i in range(len(stores_to_visit)) for j in range(len(stores_to_visit)) if i != j]
    # Use pool.starmap() to execute process_state_stores() with each set of arguments
    pool.starmap(process_state_stores, args_list)
    pool.close()
    pool.join()
    get_output = pd.read_csv("stores_output.csv")
    results = get_output.values.tolist()
    find_missing_results(stores_to_visit, results)
    
    args_list_depot = [(i, depot_df, stores_to_visit) for i in range(len(stores_to_visit))]
    pool.starmap(process_state_depot_stores, args_list_depot)
    pool.close()
    pool.join()
    get_output = pd.read_csv("depot_output.csv")
    depot_results = get_output.values.tolist()
    find_missing_results(depot_df, depot_results)
    
    
    print('All processes completed in ', time.time() - start_time, ' seconds')

if __name__ == "__main__":
    main()
