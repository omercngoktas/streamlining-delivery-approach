import mysql.connector

host = "localhost"
user = "root"
password = "9113"
database_name = "RetailLogisticsDB"

# Connecting to the database
db_conn = mysql.connector.connect(
    host=host,
    user=user,
    password=password
)

# creating cursor
cursor = db_conn.cursor()

# creating a new database
try:
    cursor.execute(f"DROP DATABASE {database_name};")
    cursor.execute(f"CREATE DATABASE {database_name};")
    print(f"Database: {database_name} has created successfully.")
except mysql.connector.Error as err:
    print(err)

# using the database
cursor.execute(f"USE {database_name};")

# creating tables
cursor.execute("""
    -- Creating table for depots
    CREATE TABLE depots (
        depot_id VARCHAR(23) PRIMARY KEY,
        depot_location VARCHAR(255),
        depot_address VARCHAR(255),
        latitude DECIMAL(10, 7),
        longitude DECIMAL(10, 7)
    );
""")

cursor.execute("""
    -- Creating table for stores
    CREATE TABLE stores (
        store_id VARCHAR(23) PRIMARY KEY,
        store_location VARCHAR(255),
        store_address VARCHAR(255),
        latitude DECIMAL(10, 7),
        longitude DECIMAL(10, 7),
        depot_id VARCHAR(23),
        FOREIGN KEY (depot_id) REFERENCES depots (depot_id)
    );  
""")

cursor.execute("""
    -- This table is for sales_rates
    CREATE TABLE sales_rates (
        store_id VARCHAR(23),
        sales_rate DECIMAL(10, 2),
        FOREIGN KEY (store_id) REFERENCES stores(store_id)
    );
""")

cursor.execute("""
    -- Shipments' history will be in this table
    CREATE TABLE shipments (
        shipment_id CHAR(36) PRIMARY KEY,
        store_id VARCHAR(23),
        date DATE,
        shipments INT,
        FOREIGN KEY (store_id) REFERENCES stores(store_id)
    );
""")

        

print("Tables created successfully.")


# Liste of tables in the database
cursor.execute("SHOW TABLES;")
tables = cursor.fetchall()

print("Mevcut tablolar:")
for table in tables:
    print(table)
