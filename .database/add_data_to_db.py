import mysql.connector
import csv

# CSV dosyasının adı
depots_csv = '/home/omercngoktas/github/routing-and-distribution-optimization/data/depots.csv'
sales_history_csv = '/home/omercngoktas/github/routing-and-distribution-optimization/data/sales_history.csv'
sales_rates_csv = '/home/omercngoktas/github/routing-and-distribution-optimization/data/sales_rates.csv'
stores_csv = '/home/omercngoktas/github/routing-and-distribution-optimization/data/stores.csv'

# MySQL bağlantısını oluştur
db_conn = mysql.connector.connect(
    host="localhost",
    user="root",
    password="9113",
    database="RetailLogisticsDB"
)

cursor = db_conn.cursor()

# CSV dosyasını aç ve verileri oku
with open(depots_csv, 'r') as file:
    csv_reader = csv.DictReader(file)
    
    for row in csv_reader:
        depot_id = row['depot_id']
        depot_location = row['depot_location']
        depot_address = row['depot_address']
        latitude = row['latitude']
        longitude = row['longitude']
        
        sql = "INSERT INTO depots (depot_id, depot_location, depot_address, latitude, longitude) VALUES (%s, %s, %s, %s, %s)"
        values = (depot_id, depot_location, depot_address, latitude, longitude)
        
        cursor.execute(sql, values)
        db_conn.commit()

# CSV dosyasını aç ve verileri oku
with open(stores_csv, 'r') as file:
    csv_reader = csv.DictReader(file)
    
    for row in csv_reader:
        store_id = row['store_id']
        store_location = row['store_location']
        store_address = row['store_address']
        latitude = row['latitude']
        longitude = row['longitude']
        depot_id = row['depot_id']
        
        sql = "INSERT INTO stores (store_id, store_location, store_address, latitude, longitude, depot_id) VALUES (%s, %s, %s, %s, %s)"
        values = (store_id, store_location, store_address, latitude, longitude, depot_id)
        
        cursor.execute(sql, values)
        db_conn.commit()
        
# CSV dosyasını aç ve verileri oku
with open(sales_history_csv, 'r') as file:
    csv_reader = csv.DictReader(file)
    
    for row in csv_reader:
        shipment_id = row['shipment_id']
        store_id = row['store_id']
        date = row['date']
        shipments = row['shipments']
        
        # Verileri tabloya eklemek için INSERT INTO sorgusu
        insert_query = """
            INSERT INTO shipments (shipment_id, store_id, date, shipments)
            VALUES (%s, %s, %s, %s)
        """
        
        cursor.execute(insert_query, (shipment_id, store_id, date, shipments))

        # Değişiklikleri kaydet
        db_conn.commit()


# Bağlantıyı kapat
db_conn.close()
