from db_connection import DBConnection
import pandas as pd

# return csv file
def get_data(file_path):
    try:
        data = pd.read_csv(file_path)
        return data
    
    except Exception as e:
        print(e)
        return None

def insert_data_from_csv(db, depots, stores, vehicles, shipments_history, sales_rates):
    if db.is_db_created_recently:
        return
    
    # Insert data into the depots table from depot.csv
    for i in range(len(depots)):
        depot_id = depots.loc[i, "depot_id"]
        depot_location = depots.loc[i, "depot_location"]
        depot_address = depots.loc[i, "depot_address"]
        latitude = depots.loc[i, "latitude"]
        longitude = depots.loc[i, "longitude"]
        db.insert("depots", ["depot_id", "depot_location", "depot_address", "latitude", "longitude"], [depot_id, depot_location, depot_address, latitude, longitude])
        
    # Insert data into the stores table from stores.csv
    for i in range(len(stores)):
        store_id = stores.loc[i, "store_id"]
        store_location = stores.loc[i, "store_location"]
        store_address = stores.loc[i, "store_address"]
        latitude = stores.loc[i, "latitude"]
        longitude = stores.loc[i, "longitude"]
        depot_id = stores.loc[i, "depot_id"]
        db.insert("stores", ["store_id", "store_location", "store_address", "latitude", "longitude", "depot_id"], [store_id, store_location, store_address, latitude, longitude, depot_id])
        
    # Insert data into the vehicles table from vehicles.csv
    for i in range(len(vehicles)):
        vehicle_id = vehicles.loc[i, "vehicle_id"]
        depot_id = vehicles.loc[i, "depot_id"]
        palette_capacity = int(vehicles.loc[i, "palette_capacity"])
        status = vehicles.loc[i, "status"]
        db.insert("vehicles", ["vehicle_id", "depot_id", "palette_capacity", "status"], [vehicle_id, depot_id, palette_capacity, status])
        
    # Insert data into the shipment_history table from shipment_history.csv
    for i in range(len(shipments_history)):
        shipment_id = shipments_history.loc[i, "shipment_id"]
        store_id = shipments_history.loc[i, "store_id"]
        date = shipments_history.loc[i, "date"]
        shipments = int(shipments_history.loc[i, "shipments"])
        db.insert("shipment_history", ["shipment_id", "store_id", "date", "shipments"], [shipment_id, store_id, date, shipments])
        
    # Insert data into the sales_rates table from sales_rates.csv
    for i in range(len(sales_rates)):
        store_id = sales_rates.loc[i, "store_id"]
        sales_rate = int(sales_rates.loc[i, "sales_rate"])
        db.insert("sales_rates", ["store_id", "sales_rate"], [store_id, sales_rate])
        
    print("Data inserted successfully")
    

def main():
    # Create an object of the DBConnection class
    db = DBConnection('postgres', '9113', 'localhost', '5432')
    db.set_dbname('gratis')
    
    depots = get_data('./data/depot/depots.csv')
    stores = get_data('./data/store/stores.csv')
    vehicles = get_data('./data/vehicle/vehicles.csv')
    shipments_history = get_data('./data/shipment_history/shipment_history.csv')
    sales_rates = get_data('./data/shipment_history/sales_rates.csv')
    insert_data_from_csv(db, depots, stores, vehicles, shipments_history, sales_rates)
    
if __name__ == "__main__":
    main()
