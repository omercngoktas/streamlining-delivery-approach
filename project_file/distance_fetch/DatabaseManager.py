import psycopg2
from psycopg2 import sql
import pandas as pd

class DBConnection:
    def __init__(self, user, password, host, port, dbname):
        self.__user = user
        self.__password = password
        self.__dbname = dbname
        self.__host = host
        self.__port = port
        self.conn = None
        self.cursor = None
        
    # getter and setter methods
    def get_user(self):
        return self.__user

    def set_user(self, user):
        self.__user = user
        
    def get_password(self):
        return self.__password
    
    def set_password(self, password):
        self.__password = password
        
    def get_dbname(self):
        return self.__dbname
    
    def set_dbname(self, dbname):
        self.__dbname = dbname
        
    def get_host(self):
        return self.__host
    
    def set_host(self, host):
        self.__host = host
        
    def get_port(self):
        return self.__port
    
    def set_port(self, port):
        self.__port = port
    
    # Connect to PostgreSQL server, and check if the connection is successful
    def connect(self):
        try:
            self.conn = psycopg2.connect(
                user = self.__user,
                password = self.__password,
                host = self.__host,
                port = self.__port
            )
            self.conn.autocommit = True
            self.cursor = self.conn.cursor()
            print("Connected to the PostgreSQL server successfully")
            self.connect_to_db()
            
        except Exception as e:
            self.create_database(self.__dbname)
            print(f"Error: {e}")
    
    # Connect to the database
    def connect_to_db(self):
        try:
            self.conn = psycopg2.connect(
                user = self.__user,
                password = self.__password,
                host = self.__host,
                port = self.__port,
                dbname = self.__dbname
            )
            self.conn.autocommit = True
            self.cursor = self.conn.cursor()
        
        except Exception as e:
            print(f"Error: {e}")
            self.create_database()
        
    # Create a database
    def create_database(self):
        try:
            self.cursor.execute(sql.SQL("CREATE DATABASE {}").format(sql.Identifier(self.__dbname)))
            print(f"Database {self.__dbname} created successfully")
            
            self.conn = psycopg2.connect(
                user = self.__user,
                password = self.__password,
                host = self.__host,
                port = self.__port,
                dbname = self.__dbname
            )
            self.conn.autocommit = True
            self.cursor = self.conn.cursor()
            self.create_tables()
            self.insert_data_from_csv()
            
        except Exception as e:
            print(f"Error: {e}")
            
    def close(self):
        self.cursor.close()
        self.conn.close()
        print("Connection closed")
    
    # Create a table if it does not exist and use the list of columns with adding to string
    def create_table(self, table_name, columns):
        try:
            query = "CREATE TABLE IF NOT EXISTS " + table_name + " (" + ", ".join(columns) + ");"
            self.cursor.execute(query)    
            print(f"Table {table_name} created successfully")
            
        except Exception as e:
            print(f"Error: {e}")
    
    # creating the tables if they do not exist
    def create_tables(self):
        # depots table
        self.create_table(
            "depots", [
                "depot_id TEXT PRIMARY KEY", 
                "depot_location VARCHAR(256)", 
                "depot_address VARCHAR(256)",
                "latitude FLOAT",
                "longitude FLOAT"
            ]
        )
        
        # stores table
        self.create_table(
            "stores", [
                "store_id TEXT PRIMARY KEY",
                "store_location VARCHAR(256)", 
                "store_address VARCHAR(256)",
                "latitude FLOAT", 
                "longitude FLOAT",
                "depot_id TEXT",
                "sales_rate INTEGER",
                "FOREIGN KEY (depot_id) REFERENCES depots(depot_id)"
            ]
        )
        
        # vehicles table
        self.create_table(
            "vehicles", [
                "vehicle_id TEXT PRIMARY KEY", 
                "depot_id TEXT", 
                "palette_capacity INTEGER", 
                "status VARCHAR(256)", 
                "FOREIGN KEY (depot_id) REFERENCES depots(depot_id)"
            ]
        )
        
        # shipment_history table
        self.create_table(
            "shipment_history", [
                "shipment_id TEXT PRIMARY KEY", 
                "store_id TEXT", 
                "date DATE", "shipments INTEGER", 
                "FOREIGN KEY (store_id) REFERENCES stores(store_id)"
            ]
        )
        
        # stores_distances_durations table
        self.create_table(
            "stores_distances_durations", [
                "st_dist_dura_id TEXT PRIMARY KEY",
                "current_id TEXT REFERENCES stores(store_id) ON DELETE CASCADE", 
                "current_latitude FLOAT", 
                "current_longitude FLOAT",
                "duration VARCHAR(50)", 
                "distance VARCHAR(50)", 
                "next_id TEXT REFERENCES stores(store_id) ON DELETE CASCADE","next_latitude FLOAT",
                "next_longitude FLOAT"
            ]
        )
        
        # depots_distances_durations table
        self.create_table(
            "depots_distances_durations", [
                "dp_dist_dura_id TEXT PRIMARY KEY", 
                "depot_id TEXT REFERENCES depots(depot_id) ON DELETE CASCADE", 
                "depot_latitude FLOAT",
                "depot_longitude FLOAT","duration VARCHAR(50)", 
                "distance VARCHAR(50)", 
                "next_id TEXT REFERENCES stores(store_id) ON DELETE CASCADE","next_latitude FLOAT",
                "next_longitude FLOAT"
            ]
        )
        
    # Insert data into the table
    def insert(self, table_name, columns, values):
        try:
            query = "INSERT INTO " + table_name + " (" + ", ".join(columns) + ") VALUES (" + ", ".join(["%s" for _ in values]) + ");"
            self.cursor.execute(query, values)
            print(f"Record inserted successfully")
            
        except Exception as e:
            print(f"Error: {e}")

    # Select data from the table
    def select(self, table_name, columns):
        try:
            query = "SELECT " + ", ".join(columns) + " FROM " + table_name + ";"
            self.cursor.execute(query)
            return self.cursor.fetchall()
        
        except Exception as e:
            print(f"Error: {e}")
            return None
    
    # insert shipment history
    def insert_shipment_history(self, shipment_id, store_id, date, shipments):
        try:
            query = "INSERT INTO shipment_history (shipment_id, store_id, date, shipments) VALUES (%s, %s, %s, %s);"
            self.cursor.execute(query, (shipment_id, store_id, date, shipments))
            print(f"Record inserted successfully")
            
        except Exception as e:
            print(f"Error: {e}")
    
    def insert_data_from_csv(self):
        depots = pd.read_csv('./data/depot/depots.csv')
        stores = pd.read_csv('./data/store/stores.csv')
        vehicles = pd.read_csv('./data/vehicle/vehicles.csv')
        shipments_history = pd.read_csv('./data/shipment_history/shipment_history.csv')
        depot_output = pd.read_csv('./data/output/depot_output.csv')
        stores_output = pd.read_csv('./data/output/stores_output.csv')
        
        # Insert data into the depots table from depot.csv
        for i in range(len(depots)):
            depot_id = depots.loc[i, "depot_id"]
            depot_location = depots.loc[i, "depot_location"]
            depot_address = depots.loc[i, "depot_address"]
            latitude = depots.loc[i, "latitude"]
            longitude = depots.loc[i, "longitude"]
            self.insert("depots", ["depot_id", "depot_location", "depot_address", "latitude", "longitude"], [depot_id, depot_location, depot_address, latitude, longitude])
            
        # Insert data into the stores table from stores.csv
        for i in range(len(stores)):
            store_id = stores.loc[i, "store_id"]
            store_location = stores.loc[i, "store_location"]
            store_address = stores.loc[i, "store_address"]
            latitude = stores.loc[i, "latitude"]
            longitude = stores.loc[i, "longitude"]
            depot_id = stores.loc[i, "depot_id"]
            sales_rate = int(stores.loc[i, "sales_rate"])
            self.insert("stores", ["store_id", "store_location", "store_address", "latitude", "longitude", "depot_id", "sales_rate"], [store_id, store_location, store_address, latitude, longitude, depot_id, sales_rate])
            
        # Insert data into the vehicles table from vehicles.csv
        for i in range(len(vehicles)):
            vehicle_id = vehicles.loc[i, "vehicle_id"]
            depot_id = vehicles.loc[i, "depot_id"]
            palette_capacity = int(vehicles.loc[i, "palette_capacity"])
            status = vehicles.loc[i, "status"]
            self.insert("vehicles", ["vehicle_id", "depot_id", "palette_capacity", "status"], [vehicle_id, depot_id, palette_capacity, status])
            
        # Insert data into the shipment_history table from shipment_history.csv
        for i in range(len(shipments_history)):
            shipment_id = shipments_history.loc[i, "shipment_id"]
            store_id = shipments_history.loc[i, "store_id"]
            date = shipments_history.loc[i, "date"]
            shipments = int(shipments_history.loc[i, "shipments"])
            self.insert("shipment_history", ["shipment_id", "store_id", "date", "shipments"], [shipment_id, store_id, date, shipments])
        
        try:
            # Insert data into the stores_distances_durations table from stores_output.csv
            for i in range(len(stores_output)):
                st_dist_dura_id = stores_output.loc[i, "id"]
                current_id = stores_output.loc[i, "current_id"]
                current_latitude = stores_output.loc[i, "current_latitude"]
                current_longitude = stores_output.loc[i, "current_longitude"]
                duration = stores_output.loc[i, "duration"]
                distance = stores_output.loc[i, "distance"]
                next_id = stores_output.loc[i, "next_id"]
                next_latitude = stores_output.loc[i, "next_latitude"]
                next_longitude = stores_output.loc[i, "next_longitude"]
                self.insert("stores_distances_durations", ["st_dist_dura_id", "current_id", "current_latitude", "current_longitude", "duration", "distance", "next_id", "next_latitude", "next_longitude"], [st_dist_dura_id, current_id, current_latitude, current_longitude, duration, distance, next_id, next_latitude, next_longitude])

            # Insert data into the depots_distances_durations table from depot_output.csv
            for i in range(len(depot_output)):
                dp_dist_dura_id = depot_output.loc[i, "id"]
                depot_id = depot_output.loc[i, "current_id"]
                depot_latitude = depot_output.loc[i, "current_latitude"]
                depot_longitude = depot_output.loc[i, "current_longitude"]
                duration = depot_output.loc[i, "duration"]
                distance = depot_output.loc[i, "distance"]
                next_id = depot_output.loc[i, "next_id"]
                next_latitude = depot_output.loc[i, "next_latitude"]
                next_longitude = depot_output.loc[i, "next_longitude"]
                self.insert("depots_distances_durations", ["dp_dist_dura_id", "depot_id", "depot_latitude", "depot_longitude", "duration", "distance", "next_id", "next_latitude", "next_longitude"], [dp_dist_dura_id, depot_id, depot_latitude, depot_longitude, duration, distance, next_id, next_latitude, next_longitude])
                
        except Exception as e:
            print("An error occured:", e)
        
        print("Data inserted successfully")

# def main():
#     db = DBConnection('postgres', '9113', 'localhost', '5432', 'gratis')
#     db.connect()
    
# if __name__ == "__main__":
#     main()
