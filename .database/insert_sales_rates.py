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

sales_rates_csv = '/home/omercngoktas/github/routing-and-distribution-optimization/data/sales_rates.csv'

# CSV dosyasını aç ve verileri oku
with open(sales_rates_csv, 'r') as file:
    csv_reader = csv.DictReader(file)
    
    for row in csv_reader:
        store_id = row['store_id']
        sales_rate = row['sales_rate']
        
        # Verileri tabloya eklemek için INSERT INTO sorgusu
        insert_query = """
            INSERT INTO sales_rates (store_id, sales_rate)
            VALUES (%s, %s)
        """
        
        cursor.execute(insert_query, (store_id, sales_rate))

        # Değişiklikleri kaydet
        db_conn.commit()

# Bağlantıyı kapat
db_conn.close()
