# For all given stores, it creates a history between start-end dates.
# It uses probability for each sales_rate. So, distribution will change accordingly.

import pandas as pd
from datetime import date, timedelta
import numpy as np
import uuid
import os

def get_probabilities():
    probabilities = {
        1: [0.40, 0.30, 0.20, 0.10, 0.00, 0.00, 0.00],
        2: [0.40, 0.10, 0.30, 0.15, 0.05, 0.00, 0.00],
        3: [0.30, 0.05, 0.10, 0.35, 0.20, 0.00, 0.00],
        4: [0.40, 0.00, 0.05, 0.15, 0.30, 0.05, 0.05],
        5: [0.40, 0.00, 0.00, 0.05, 0.15, 0.30, 0.10],
        6: [0.50, 0.00, 0.00, 0.05, 0.10, 0.15, 0.20]
    }
    return probabilities

def generate_shipment_history(probabilities, date_format, start_date, stores, end_date=date.today(), shipment_history_path="shipment_history.csv"):
    
    if os.path.exists(shipment_history_path):
        existing_shipment_history = pd.read_csv(shipment_history_path)
        start_date = existing_shipment_history["date"].max()
        start_date = pd.to_datetime(start_date, format=date_format)
        start_date += pd.DateOffset(days=1)
        print("Start date:", start_date)
    
    shipment_history = []

    for index, row in stores.iterrows():
        store_id = row['store_id']
        sales_rate = row['sales_rate']
        
        weights = probabilities[sales_rate]
        
        current_date = pd.to_datetime(start_date, format=date_format)
        end_date = pd.to_datetime(end_date, format=date_format)
        
        while current_date <= end_date:
            date_str = current_date.strftime(date_format)

            daily_shipments = np.random.choice(np.arange(0, 7), p=weights)
            
            if daily_shipments < 0:
                daily_shipments = 0
                
            shipment_id = str(uuid.uuid4())
            shipment_data = {"shipment_id": shipment_id, "store_id": store_id, "date": date_str, "shipments": daily_shipments}
            shipment_history.append(shipment_data)
            current_date += pd.DateOffset(days=1)
    
    shipment_history_df = pd.DataFrame(shipment_history)
    
    # recently created shipment history is saved to a new file to insert into the database later
    try:
        # new_shipment_history_df = shipment_history_df.copy()
        # print(shipment_history_df.head())
        print("New shipment history generated till toda12312312312312345r23456y.")
        # new_shipment_history_df["date"] = pd.to_datetime(new_shipment_history_df["date"])
        # new_shipment_history_df = new_shipment_history_df.sort_values(by=["date", "shipment_id"])
        # new_shipment_history_df.to_csv("../data/shipment_history/new_shipment_history.csv", index=False)    
    except Exception as e:
        print("Error while saving new shipment history1:", e)
    
    if os.path.exists(shipment_history_path):
        shipment_history_df = pd.concat([existing_shipment_history, shipment_history_df])
    
    return shipment_history_df

def generate_history_till_today():
    shipment_history_path = "../data/shipment_history/shipment_history.csv"
    stores = pd.read_csv("../data/store/stores.csv")
    probabilities = get_probabilities()
    start_date = "2015-01-01"
    end_date = date.today()
    # end_date = "2020-10-01"
    date_format = "%Y-%m-%d"
    
    shipment_history_df = generate_shipment_history(probabilities, date_format, start_date, stores, end_date=end_date, shipment_history_path=shipment_history_path)
    
    shipment_history_df["date"] = pd.to_datetime(shipment_history_df["date"])
    shipment_history_df = shipment_history_df.sort_values(by=["date", "shipment_id"])
    shipment_history_df.to_csv(shipment_history_path, index=False)
    print("Shipment history generated till today.")