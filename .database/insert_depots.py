import mysql.connector
import csv

# CSV dosyasının adı
depots_csv = '/home/omercngoktas/github/routing-and-distribution-optimization/data/depots.csv'

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
    
db_conn.close()