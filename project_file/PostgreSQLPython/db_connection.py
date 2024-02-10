import psycopg2
from psycopg2 import sql

class DBConnection:
    def __init__(self, user, password, host, port):
        self.__user = user
        self.__password = password
        self.__dbname = None
        self.__host = host
        self.__port = port
        self.conn = None
        self.cursor = None
        self.is_db_created_recently = False
        # Connect to the PostgreSQL server
        self.connect()
        
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
        self.create_database(dbname)
        self.connect()
        self.create_tables()
        
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
                dbname = self.__dbname,
                user = self.__user,
                password = self.__password,
                host = self.__host,
                port = self.__port
            )
            self.conn.autocommit = True
            self.cursor = self.conn.cursor()
            print("Connected to the database")
        except Exception as e:
            print(f"Error: {e}")

    # Create a database
    def create_database(self, dbname):
        try:
            self.cursor.execute(sql.SQL("CREATE DATABASE {}").format(sql.Identifier(dbname)))
            # print(f"Database {dbname} created successfully")
        except Exception as e:
            self.is_db_created_recently = True
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
        self.create_table("depots", ["depot_id TEXT PRIMARY KEY", "depot_location VARCHAR(256)", "depot_address VARCHAR(256)", "latitude FLOAT", "longitude FLOAT"])
        self.create_table("stores", ["store_id TEXT PRIMARY KEY", "store_location VARCHAR(256)", "store_address VARCHAR(256)", "latitude FLOAT", "longitude FLOAT", "depot_id TEXT", "FOREIGN KEY (depot_id) REFERENCES depots(depot_id)"])
        self.create_table("vehicles", ["vehicle_id TEXT PRIMARY KEY", "depot_id TEXT", "palette_capacity INTEGER", "status VARCHAR(256)", "FOREIGN KEY (depot_id) REFERENCES depots(depot_id)"])
        self.create_table("sales_rates", ["store_id TEXT", "sales_rate INTEGER", "FOREIGN KEY (store_id) REFERENCES stores(store_id)"])
        self.create_table("shipment_history", ["shipment_id TEXT PRIMARY KEY", "store_id TEXT", "date DATE", "shipments INTEGER", "FOREIGN KEY (store_id) REFERENCES stores(store_id)"])
        self.create_table("stores_distances_durations", ["current_id TEXT REFERENCES stores(store_id) ON DELETE CASCADE", "current_latitude FLOAT", "current_longitude FLOAT","duration FLOAT", "distance FLOAT", "next_id TEXT REFERENCES stores(store_id) ON DELETE CASCADE","next_latitude FLOAT","next_longitude FLOAT"])
        self.create_table("depots_distances_durations", ["depot_id TEXT REFERENCES depots(depot_id) ON DELETE CASCADE", "depot_latitude FLOAT", "depot_longitude FLOAT","duration FLOAT", "distance FLOAT", "next_id TEXT REFERENCES stores(store_id) ON DELETE CASCADE","next_latitude FLOAT","next_longitude FLOAT"])
        
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