# For all given stores, it creates a history between start-end dates.
# It uses probability for each sales_rate. So, distribution will change accordingly.

import random
import pandas as pd
from datetime import date, timedelta
import numpy as np

def get_probabilities():
    probabilities = {
        #       0      1      2       3       4       5       6
        1:  [0.40,   0.30,  0.20,   0.10,   0.00,   0.00,   0.00],
        2:  [0.40,   0.10,  0.30,   0.15,   0.05,   0.00,   0.00],
        3:  [0.30,   0.05,  0.10,   0.35,   0.20,   0.00,   0.00],
        4:  [0.40,   0.00,  0.05,   0.15,   0.30,   0.05,   0.05],
        5:  [0.40,   0.00,  0.00,   0.05,   0.15,   0.30,   0.10],
        6:  [0.50,   0.00,  0.00,   0.05,   0.10,   0.15,   0.20]
    }
    
    return probabilities

def generate_sales_history(probabilities, date_format, start_date, sales_rates_df, end_date=date.today()):
    sales_history = []

    for index, row in sales_rates_df.iterrows():
        store_id = row['store_id']
        sales_rate = row['sales_rate']
        
        weights = probabilities[sales_rate]
        
        # Her mağaza için günlük sevkiyat verileri
        current_date = pd.to_datetime(start_date, format=date_format)
        end_date = pd.to_datetime(end_date, format=date_format)
        while current_date <= end_date:
            # O gün için sevkiyat verisi
            date_str = current_date.strftime(date_format)

            # O gün için üretilen sevkiyat değeri
            daily_shipments = np.random.choice(np.arange(0, 7), p=weights)
            
            if daily_shipments < 0:
                daily_shipments = 0
                
            shipment_data = {"store_id": store_id, "date": date_str, "shipments": daily_shipments}
            # Sevkiyat verisini listeye ekle
            sales_history.append(shipment_data)
            # Bir sonraki gün
            current_date += pd.DateOffset(days=1)
            
    # Oluşturulan sevkiyat geçmişini bir DataFrame'e dönüştür
    sales_history_df = pd.DataFrame(sales_history)
    return sales_history_df

def main():
    sales_rates_df = pd.read_csv("sales_rates.csv")
    stores_df = pd.read_csv("stores.csv")
    probabilities = get_probabilities()
    start_date = "2018-01-01"
    # end_date = "2023-09-27"
    end_date = date.today()
    date_format = "%Y-%m-%d"
    
    # Üretilen satış geçmişleri sales_history_df içerisinde tutuluyor.
    sales_history_df = generate_sales_history(probabilities, date_format, start_date, sales_rates_df)
    
    # Üretilen satış geçmişleri tarihe göre sıralanıyor burada.
    sales_history_df["date"] = pd.to_datetime(sales_history_df["date"])  # Tarihi datetime nesnelerine dönüştürün
    sales_history_df = sales_history_df.sort_values(by="date")  # Tarihe göre sıralama yapın
    # Dosyaya kaydedilir.
    sales_history_df.to_csv("sales_history.csv", index=False)

if __name__ == "__main__":
    main()