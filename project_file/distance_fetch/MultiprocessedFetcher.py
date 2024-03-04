import os
import pandas as pd
from GoogleMaps import GoogleMapsHandler
import multiprocessing

class MultiprocessHandler:
    def __init__(self, cpu_count, stores, current_store=None, depot=None, stores_with_shipment=None):
        self.cpu_count = cpu_count
        self.stores = stores
        self.stores_with_shipment = stores_with_shipment
        self.current_store = current_store
        self.depot = depot
        
    def process_state_stores(self, current_store, next_store):
        google_maps_handler = GoogleMapsHandler()
        travel_time, distance = google_maps_handler.fetch_duration_and_distance(current_store['latitude'], current_store['longitude'], next_store['latitude'], next_store['longitude'])
        output_df = pd.DataFrame([[current_store['store_id'], current_store['latitude'], current_store['longitude'], travel_time, distance, next_store['store_id'], next_store['latitude'], next_store['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
        file_lock = multiprocessing.Lock()
        file_lock.acquire()
        try:
            # Check if any element of the output_df is None
            if output_df.isnull().values.any():
                print("Skipping write to file")
                return

            print("Output: ", output_df.values.tolist())
            output_df.to_csv("../data/output/stores_output.csv", mode="a", index=False, header=not os.path.exists("../data/output/stores_output.csv"))
            print("Wrote to file")
        finally:
            # Release file lock after writing to the file
            google_maps_handler.close_driver()
            file_lock.release()

    def process_state_depot(self, depot, current_store):
        google_maps_handler = GoogleMapsHandler()
        travel_time, distance = google_maps_handler.fetch_duration_and_distance(depot['latitude'], depot['longitude'], current_store['latitude'], current_store['longitude'])
        output_df = pd.DataFrame([[depot['depot_id'], depot['latitude'], depot['longitude'], travel_time, distance, current_store['store_id'], current_store['latitude'], current_store['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
        file_lock = multiprocessing.Lock()
        # Acquire file lock before writing to the file
        file_lock.acquire()
        try:
            # Check if any element of the output_df is None
            if output_df.isnull().values.any():
                print("Skipping write to file")
                return

            print("Output: ", output_df.values.tolist())
            output_df.to_csv("../data/output/depot_output.csv", mode="a", index=False, header=not os.path.exists("../data/output/depot_output.csv"))
            print("Wrote to file")
        finally:
            # Release file lock after writing to the file
            google_maps_handler.close_driver()
            file_lock.release()

    def create_pool_for_stores(self):
        print("Creating pool for store ", self.current_store["store_id"])
        
        args_list = []
        
        for index, next_store in self.stores_with_shipment.iterrows():
            if next_store["store_id"] != self.current_store["store_id"]:
                args_list.append((self.current_store, next_store))
        
        pool = multiprocessing.Pool(processes=self.cpu_count)
        pool.starmap(self.process_state_stores, args_list)
        pool.close()
        pool.join()

    def create_pool_for_depot(self):
        print("Creating pool for depot ", self.depot["depot_id"])
        args_list = []
        
        for index, next_store in self.stores_with_shipment.iterrows():
            args_list.append((self.depot, next_store))
        
        pool = multiprocessing.Pool(processes=self.cpu_count)
        pool.starmap(self.process_state_depot, args_list)
        pool.close()
        pool.join()