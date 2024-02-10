from GoogleMaps import GoogleMapsHandler
from LocationManager import LocationManager
from ShipmentManager import ShipmentManager
from MultiprocessedFetcher import MultiprocessHandler
import pandas as pd
import time
import datetime
import uuid

# return the stores which have shipment
def get_stores_with_shipments(stores, stores_with_shipment):
    arranged_stores = []
    # if any of the stores in stores is in stores_with_shipment, then add it to arranged_stores
    for index, row in stores.iterrows():
        if row["store_id"] in stores_with_shipment.values:
            arranged_stores.append([row["store_id"], row["latitude"], row["longitude"]])
    
    return arranged_stores

def find_missing_results(stores, results):
    missing_results = []
    for i in range(len(stores)):
        for j in range(len(stores)):
            if i != j:
                if not any(result[0] == stores.iloc[i]['store_id'] and result[5] == stores.iloc[j]['store_id'] for result in results):
                    missing_results.append([stores.iloc[i]['store_id'], stores.iloc[j]['store_id']])
        
    return missing_results

def create_csv_files(output_path):
    output_df = pd.DataFrame(columns=[
                                'current_id', 
                                'current_latitude', 
                                'current_longitude', 
                                'duration', 
                                'distance', 
                                'next_id', 
                                'next_latitude', 
                                'next_longitude'
                            ])
    
    output_df.to_csv(f"{output_path}/stores_output.csv", index=False)
    output_df.to_csv(f"{output_path}/depot_output.csv", index=False)

def distance_duration_id_generator(year, month, day):
    generated_uuid = uuid.uuid4()
    # Convert UUID to hexadecimal string
    hex_uuid = generated_uuid.hex
    # Truncate the hexadecimal string to the desired length
    truncated_uuid = hex_uuid[:20]
    return f"DD-{truncated_uuid.upper()}-{year}-{month}-{day}"


def main():
    start_time = time.time()
    today = datetime.date.today()
    day = 5
    month = 2
    year = 2023
    cpu_count = 12
    output_path = "./output"
    
    shipment_manager = ShipmentManager("./data/shipment_history/shipment_history.csv", "./output/stores_with_shipment.csv")
    location_manager = LocationManager("./data/store/stores.csv", "./data/depot/depots.csv")
        
    shipment_manager.get_days_shipment_output(5, 2, 2023)
    
    # depot_df = location_manager.get_depot() # get the depot
    # stores = location_manager.get_stores() # get the stores
    # stores_with_shipment = location_manager.get_stores(f"{output_path}/stores_with_shipment.csv") # get the stores with shipment
    
    # stores_shipments_with_details = get_stores_with_shipments(stores, stores_with_shipment) # get the stores with shipment
    # stores_to_visit = pd.DataFrame(stores_shipments_with_details, columns=["store_id", "latitude", "longitude"]) # create a dataframe of the stores with shipment
    # create_csv_files(output_path)
    
    # multiprocess_handler = MultiprocessHandler(cpu_count=cpu_count, stores=stores, depot=depot_df.iloc[0], stores_with_shipment=stores_to_visit)
    
    # for index, current_store in stores_to_visit.iterrows():
    #     multiprocess_handler.current_store = current_store
    #     multiprocess_handler.create_pool_for_stores()
    
    # get_output = pd.read_csv(f"{output_path}/stores_output.csv")
    # results = get_output.values.tolist()
    # stores_missing_result = find_missing_results(stores_to_visit, results)
    # pd.DataFrame(stores_missing_result, columns=["current_id", "next_id"]).to_csv(f"{output_path}/stores_missing_result.csv", index=False)
    
    # multiprocess_handler.create_pool_for_depot()
    # get_output = pd.read_csv(f"{output_path}/depot_output.csv")
    # depot_results = get_output.values.tolist()
    # depot_missing_result = find_missing_results(depot_df, depot_results)
    # pd.DataFrame(depot_missing_result, columns=["current_id", "next_id"]).to_csv(f"{output_path}/depot_missing_result.csv", index=False)
    
    
    
    print('All processes completed in ', time.time() - start_time, ' seconds')
    
    # open the csv file and put id in the first column
    depot_output = pd.read_csv(f"{output_path}/depot_output.csv")
    depot_output.insert(0, 'id', [distance_duration_id_generator(year, month, day) for _ in range(len(depot_output))])
    depot_output.to_csv(f"{output_path}/depot_output.csv", index=False)
    stores_output = pd.read_csv(f"{output_path}/stores_output.csv")
    stores_output.insert(0, 'id', [distance_duration_id_generator(year, month, day) for _ in range(len(stores_output))])
    stores_output.to_csv(f"{output_path}/stores_output.csv", index=False)
    
    
if __name__ == "__main__":
    main()
