import mysql.connector
import csv

# MySQL bağlantısını oluştur
db_conn = mysql.connector.connect(
    host="localhost",
    user="root",
    password="9113",
    database="RetailLogisticsDB"
)

cursor = db_conn.cursor()

sales_history_csv = '/home/omercngoktas/github/routing-and-distribution-optimization/data/sales_history.csv'

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
