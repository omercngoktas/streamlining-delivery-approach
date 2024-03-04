import mysql.connector
import pandas as pd

class MySQL_Manager:
    def __init__(self, host, user, password, database):
        self.host = host
        self.user = user
        self.password = password
        self.database = database
        self.connection = None
        self.cursor = None

    # Connect to the database
    def connect(self):
        try:
            self.connection = mysql.connector.connect(
                host=self.host,
                user=self.user,
                password=self.password
            )
            self.cursor = self.connection.cursor()
            print("Connected to the database successfully")
            self.check_database()
            
        except Exception as e:
            print("Error: ", e)
            print("Failed to connect to the database")

    # Disconnect from the database
    def disconnect(self):
        self.cursor.close()
        self.connection.close()

    # Execute a query
    def execute(self, query):
        self.cursor.execute(query)
        self.connection.commit()

    # Fetch data from the database
    def fetch(self, query):
        self.cursor.execute(query)
        return self.cursor.fetchall()

    # Check if the database exists and if not, create it and create the tables
    def check_database(self):
        try:
            self.cursor.execute("USE {}".format(self.database))
            print("Using database '{}'".format(self.database))
        except mysql.connector.Error as e:
            self.create_database()

    # Create the database and the tables
    def create_database(self):
        try:
            self.execute("CREATE DATABASE {}".format(self.database))
            print("Database '{}' created successfully".format(self.database))
            self.execute("USE {}".format(self.database))
            print("Using database '{}'".format(self.database))
            self.create_tables()
            self.__insert_data()
            
        except mysql.connector.Error as e:
            print("Error: ", e)
            print("Failed to create database '{}'".format(self.database))
    
    # Create the tables
    def create_tables(self):
        self.create_table("depots", "depot_id VARCHAR(50) PRIMARY KEY, depot_location VARCHAR(256), depot_address VARCHAR(256), latitude FLOAT, longitude FLOAT")
        self.create_table("stores", "store_id VARCHAR(50) PRIMARY KEY, store_location VARCHAR(256), store_address VARCHAR(256), latitude FLOAT, longitude FLOAT, depot_id VARCHAR(50), sales_rate INTEGER, FOREIGN KEY (depot_id) REFERENCES depots(depot_id)")
        self.create_table("vehicles", "vehicle_id VARCHAR(50) PRIMARY KEY, depot_id VARCHAR(50), palette_capacity INTEGER, status VARCHAR(256), FOREIGN KEY (depot_id) REFERENCES depots(depot_id)")
        self.create_table("shipment_history", "shipment_id VARCHAR(50) PRIMARY KEY, store_id VARCHAR(50), date DATE, shipments INTEGER, FOREIGN KEY (store_id) REFERENCES stores(store_id)")
        self.create_table("stores_distances_durations", "st_dist_dura_id VARCHAR(50) PRIMARY KEY, date DATE, current_id VARCHAR(50) REFERENCES stores(store_id) ON DELETE CASCADE, current_latitude FLOAT, current_longitude FLOAT, duration VARCHAR(50), distance VARCHAR(50), next_id VARCHAR(50) REFERENCES stores(store_id) ON DELETE CASCADE, next_latitude FLOAT, next_longitude FLOAT")
        self.create_table("depots_distances_durations", "dp_dist_dura_id VARCHAR(50) PRIMARY KEY, date DATE, depot_id VARCHAR(50) REFERENCES depots(depot_id) ON DELETE CASCADE, depot_latitude FLOAT, depot_longitude FLOAT, duration VARCHAR(50), distance VARCHAR(50), next_id VARCHAR(50) REFERENCES stores(store_id) ON DELETE CASCADE, next_latitude FLOAT, next_longitude FLOAT")

    # Create a table
    def create_table(self, table_name, columns):
        try:
            self.execute("CREATE TABLE {} ({})".format(table_name, columns))
            print("Table '{}' created successfully".format(table_name))
        except mysql.connector.Error as e:
            print("Error: ", e)
            print("Failed to create table '{}'".format(table_name))
    
    def insert(self, table, columns, values):
        try:
            self.execute("INSERT INTO {} ({}) VALUES ({})".format(table, columns, values))
            print("Data inserted successfully")
        except mysql.connector.Error as e:
            print("Error: ", e)
            print("Failed to insert data")
            
    def select(self, table, columns):
        try:
            return self.fetch("SELECT {} FROM {}".format(columns, table))
        except mysql.connector.Error as e:
            print("Error: ", e)
            print("Failed to select data")
            
    def select_where(self, table, columns, condition):
        try:
            return self.fetch("SELECT {} FROM {} WHERE {}".format(columns, table, condition))
        except mysql.connector.Error as e:
            print("Error: ", e)
            print("Failed to select data")
            
    def __insert_data(self):
        data_path = "../data"
        depots = pd.read_csv(data_path + "/depot/depots.csv")
        stores = pd.read_csv(data_path + "/store/stores.csv")
        vehicles = pd.read_csv(data_path + '/vehicle/vehicles.csv')
        shipments_history = pd.read_csv(data_path + '/shipment_history/shipment_history.csv')
        
        # Insert depots data
        for i in range(len(depots)):
            depot_id = depots.loc[i, "depot_id"]
            depot_location = depots.loc[i, "depot_location"]
            depot_address = depots.loc[i, "depot_address"]
            latitude = depots.loc[i, "latitude"]
            longitude = depots.loc[i, "longitude"]
            self.insert("depots", "depot_id, depot_location, depot_address, latitude, longitude", "'{}', '{}', '{}', {}, {}".format(depot_id, depot_location, depot_address, latitude, longitude))
            
        # Insert stores data
        for i in range(len(stores)):
            store_id = stores.loc[i, "store_id"]
            store_location = stores.loc[i, "store_location"]
            store_address = stores.loc[i, "store_address"]
            latitude = stores.loc[i, "latitude"]
            longitude = stores.loc[i, "longitude"]
            depot_id = stores.loc[i, "depot_id"]
            sales_rate = stores.loc[i, "sales_rate"]
            self.insert("stores", "store_id, store_location, store_address, latitude, longitude, depot_id, sales_rate", "'{}', '{}', '{}', {}, {}, '{}', {}".format(store_id, store_location, store_address, latitude, longitude, depot_id, sales_rate))
        
        # Insert vehicles data
        for i in range(len(vehicles)):
            vehicle_id = vehicles.loc[i, "vehicle_id"]
            depot_id = vehicles.loc[i, "depot_id"]
            palette_capacity = vehicles.loc[i, "palette_capacity"]
            status = vehicles.loc[i, "status"]
            self.insert("vehicles", "vehicle_id, depot_id, palette_capacity, status", "'{}', '{}', {}, '{}'".format(vehicle_id, depot_id, palette_capacity, status))
            
        # Insert shipment history data
        for i in range(len(shipments_history)):
            shipment_id = shipments_history.loc[i, "shipment_id"]
            store_id = shipments_history.loc[i, "store_id"]
            date = shipments_history.loc[i, "date"]
            shipments = shipments_history.loc[i, "shipments"]
            self.insert("shipment_history", "shipment_id, store_id, date, shipments", "'{}', '{}', '{}', {}".format(shipment_id, store_id, date, shipments))
    
    # Destructor
    def __del__(self):
        self.disconnect()
        print("Destructor called, disconnected from the database")