from GoogleMaps import GoogleMapsHandler
from LocationManager import LocationManager
from ShipmentManager import ShipmentManager
from MultiprocessedFetcher import MultiprocessHandler
from MySQL_Manager import MySQL_Manager
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

# return the missing results between the stores
def find_missing_results(stores, results):
    missing_results = []
    for i in range(len(stores)):
        for j in range(len(stores)):
            if i != j:
                if not any(result[0] == stores.iloc[i]['store_id'] and result[5] == stores.iloc[j]['store_id'] for result in results):
                    missing_results.append([stores.iloc[i]['store_id'], stores.iloc[j]['store_id']])
        
    return missing_results

# create the csv files
def create_csv_files(output_path):
    output_df = pd.DataFrame(columns=['current_id','current_latitude','current_longitude','duration','distance','next_id','next_latitude','next_longitude'])
    output_df.to_csv(f"{output_path}/stores_output.csv", index=False)
    output_df.to_csv(f"{output_path}/depot_output.csv", index=False)

# generate the id for the distance and duration
def distance_duration_id_generator(year, month, day):
    generated_uuid = uuid.uuid4()
    # Convert UUID to hexadecimal string
    hex_uuid = generated_uuid.hex
    # Truncate the hexadecimal string to the desired length
    truncated_uuid = hex_uuid[:20]
    return f"DD-{truncated_uuid.upper()}-{year}-{month}-{day}"

# inserting new shipment histories to the database
def insert_new_shipment_history_to_database(db):
    # try to insert the shipment history to the database
    try: 
        # save the recently created shipment history to the database
        new_shipment_history = pd.read_csv("../data/shipment_history/new_shipment_history.csv")
        for i in range(len(new_shipment_history)):
            shipment_id = new_shipment_history.loc[i, "shipment_id"]
            store_id = new_shipment_history.loc[i, "store_id"]
            date = new_shipment_history.loc[i, "date"]
            shipments = new_shipment_history.loc[i, "shipments"]
            db.insert("shipment_history", "shipment_id, store_id, date, shipments", "'{}', '{}', '{}', {}".format(shipment_id, store_id, date, shipments))
            print("New shipment history inserted to the database: ", shipment_id, store_id, date, shipments)
            
        # delete the contents of the new_shipment_history.csv
        open("../data/shipment_history/new_shipment_history.csv", "w").close()
    except Exception as e:
        print(e)
        
def generate_id_for_distance_duration(db, year, month, day, output_path):
    # generate the id for the distance and duration and insert the results to csv
    try:
        # open the csv file and put id in the first column
        depot_output = pd.read_csv(f"{output_path}/depot_output.csv")
        depot_output.insert(0, 'id', [distance_duration_id_generator(year, month, day) for _ in range(len(depot_output))])
        depot_output.insert(1, 'date', [f"{year}-{month}-{day}" for _ in range(len(depot_output))])
        stores_output = pd.read_csv(f"{output_path}/stores_output.csv")
        stores_output.insert(0, 'id', [distance_duration_id_generator(year, month, day) for _ in range(len(stores_output))])
        stores_output.insert(1, 'date', [f"{year}-{month}-{day}" for _ in range(len(stores_output))])
        
        depot_output.to_csv(f"{output_path}/depot_output.csv", index=False)
        stores_output.to_csv(f"{output_path}/stores_output.csv", index=False)
        
        try: 
            depot_distance_duration = pd.read_csv(f"../data/distance_duration/depot_distance_duration.csv")
            stores_distance_duration = pd.read_csv(f"../data/distance_duration/stores_distance_duration.csv")
        except:
            # create the csv files
            depot_distance_duration = pd.DataFrame(columns=['id','date','current_id','current_latitude','current_longitude','duration','distance','next_id','next_latitude','next_longitude'])
            stores_distance_duration = pd.DataFrame(columns=['id','date','current_id','current_latitude','current_longitude','duration','distance','next_id','next_latitude','next_longitude'])
            
        # insert depot_output frame to depot_distance_duration frame
        depot_distance_duration = pd.concat([depot_distance_duration, depot_output], axis=0)
        depot_distance_duration.to_csv(f"../data/distance_duration/depot_distance_duration.csv", index=False)
        
        # insert stores_output frame to stores_distance_duration frame
        stores_distance_duration = pd.concat([stores_distance_duration, stores_output], axis=0)
        stores_distance_duration.to_csv(f"../data/distance_duration/stores_distance_duration.csv", index=False)
        
    except Exception as e:
        print(e)
        
    # insert the results of the depot which are duration and distance between the depot and the stores
    for index, row in depot_output.iterrows():
        db.insert("depots_distances_durations", "dp_dist_dura_id, date, depot_id, depot_latitude, depot_longitude, duration, distance, next_id, next_latitude, next_longitude", "'{}', '{}', '{}', {}, {}, '{}', '{}', '{}', {}, {}".format(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9]))
        print(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9])
        
    # insert the results of the stores which are duration and distance between the stores
    for index, row in stores_output.iterrows():
        db.insert("stores_distances_durations", "st_dist_dura_id, date, current_id, current_latitude, current_longitude, duration, distance, next_id, next_latitude, next_longitude", "'{}', '{}', '{}', {}, {}, '{}', '{}', '{}', {}, {}".format(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9]))
        print(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9])
        
def stores_missing_result_to_file(output_path, stores_to_visit):
    # get the output of the stores and the depot
    get_output = pd.read_csv(f"{output_path}/stores_output.csv")
    results = get_output.values.tolist()
    stores_missing_result = find_missing_results(stores_to_visit, results)
    pd.DataFrame(stores_missing_result, columns=["current_id", "next_id"]).to_csv(f"{output_path}/stores_missing_result.csv", index=False)
    
def depot_missing_result_to_file(output_path, depot_df):
    get_output = pd.read_csv(f"{output_path}/depot_output.csv")
    depot_results = get_output.values.tolist()
    depot_missing_result = find_missing_results(depot_df, depot_results)
    pd.DataFrame(depot_missing_result, columns=["current_id", "next_id"]).to_csv(f"{output_path}/depot_missing_result.csv", index=False)
        
def multiprocess_handler(cpu_count, stores, depot_df, stores_with_shipment):
    multiprocess_handler = MultiprocessHandler(cpu_count=cpu_count, stores=stores, depot=depot_df.iloc[0], stores_with_shipment=stores_with_shipment)
    # get the distance and duration between the stores
    for index, current_store in stores_with_shipment.iterrows():
        # set the current store
        multiprocess_handler.current_store = current_store
        # create a pool for the stores
        multiprocess_handler.create_pool_for_stores()
    
    # get the distance and duration between the depot and the stores
    multiprocess_handler.create_pool_for_depot()

def insert_distance_duration_to_database(db):
    stores_distance_duration = pd.read_csv("../data/distance_duration/stores_distance_duration.csv")
    depot_distance_duration = pd.read_csv("../data/distance_duration/depot_distance_duration.csv")
    # for index, row in stores_distance_duration.iterrows():
    #     db.insert("stores_distances_durations", "st_dist_dura_id, date, current_id, current_latitude, current_longitude, duration, distance, next_id, next_latitude, next_longitude", "'{}', '{}', '{}', {}, {}, '{}', '{}', '{}', {}, {}".format(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9]))
    #     print(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9])
        
    # for index, row in depot_distance_duration.iterrows():
    #     db.insert("depots_distances_durations", "dp_dist_dura_id, date, depot_id, depot_latitude, depot_longitude, duration, distance, next_id, next_latitude, next_longitude", "'{}', '{}', '{}', {}, {}, '{}', '{}', '{}', {}, {}".format(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9]))
    #     print(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9])

def main():
    # Database connection
    db = MySQL_Manager("localhost", "omercngoktas", "mwanamboka", "mydatabase")
    db.connect()
    
    # specifying the day to get the shipment, cpu_count, and output_path
    start_time = time.time()
    today = datetime.date.today()
    day = 19
    month = 5
    year = 2024
    cpu_count = 12
    output_path = "../data/output"
    
    # insert_distance_duration_to_database(db)
    
    # create the csv files
    create_csv_files(output_path)
    
    # ShipmentManager automatically generates the shipment history till today
    shipment_manager = ShipmentManager("../data/shipment_history/shipment_history.csv", f"{output_path}/stores_with_shipment.csv")   
    # LocationManager is used to get the depot and the stores
    location_manager = LocationManager("../data/store/stores.csv", "../data/depot/depots.csv")
    
    # try to insert the shipment history to the database
    insert_new_shipment_history_to_database(db)
    print("Recently created shipment history inserted to the database.")
    
    # given the day, month, and year, get the shipment at the given date to file
    shipment_manager.given_days_shipment_to_file(day, month, year)
    print("Shipment at the given date saved to file.")
    
    # get the depot and the stores
    depot_df = location_manager.get_depot() # get the depot
    stores = location_manager.get_stores() # get the stores
    
    # get the stores with shipment with the given date before
    stores_with_shipment = location_manager.get_stores(f"{output_path}/stores_with_shipment.csv") # get the stores with shipment
    
    # get the stores with shipment
    stores_shipments_with_details = get_stores_with_shipments(stores, stores_with_shipment) # get the stores with shipment
    stores_to_visit = pd.DataFrame(stores_shipments_with_details, columns=["store_id", "latitude", "longitude"]) # create a dataframe of the stores with shipment
    
    
    # THIS IS FETCHING THE DISTANCE AND DURATION BETWEEN THE DEPOT AND THE STORES
    # get the distance and duration between the stores and the depot
    multiprocess_handler(cpu_count, stores, depot_df, stores_to_visit)
    ############################################################################################################
    
    
    # if there are missing results, then save the missing results to a file
    stores_missing_result_to_file(output_path, stores_to_visit)
    depot_missing_result_to_file(output_path, depot_df)
    
    # generate the id for the distance and duration and insert the results to csv
    generate_id_for_distance_duration(db, year, month, day, output_path)
    
    # # delete the content of depot and stores outputs
    open(f"{output_path}/depot_output.csv", "w").close()
    open(f"{output_path}/stores_output.csv", "w").close()
    
    # print the time taken to complete the processes
    print('All processes completed in ', time.time() - start_time, ' seconds')
    
if __name__ == "__main__":
    main()
