import mysql.connector
import csv

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
with open(stores_csv, 'r') as file:
    csv_reader = csv.DictReader(file)
    
    for row in csv_reader:
        store_id = row['store_id']
        store_location = row['store_location']
        store_address = row['store_address']
        latitude = (row['latitude'])
        # Veriyi düzeltilmiş bir formata dönüştür
        longitude = row['longitude'].replace('S', '').replace('W', '-').replace('E', '').replace('N', '')

        # longitude = float(row['longitude'])
        depot_id = row['depot_id']
        
        sql = "INSERT INTO stores (store_id, store_location, store_address, latitude, longitude, depot_id) VALUES (%s, %s, %s, %s, %s, %s)"
        values = (store_id, store_location, store_address, latitude, longitude, depot_id)
        
        cursor.execute(sql, values)
        db_conn.commit()
        
db_conn.close()