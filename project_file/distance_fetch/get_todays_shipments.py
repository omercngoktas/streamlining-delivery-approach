
from datetime import date
import pandas as pd


def get_shipment_at_date(day, month, year, shipment_history_path):
    shipment_history = pd.read_csv(shipment_history_path)
    shipment_history["date"] = pd.to_datetime(shipment_history["date"])
    return shipment_history[(shipment_history["date"].dt.day == day) & (shipment_history["date"].dt.month == month) & (shipment_history["date"].dt.year == year)]

def output_stores_with_shipment(shipments):
    stores = []
    
    for index, row in shipments.iterrows():
        if row["shipments"] > 0:
            stores.append(row["store_id"])
    
    output_df = pd.DataFrame(stores, columns=["store_id"])
    output_df.to_csv("stores_with_shipment.csv", index=False)

def get_days_shipment_output(day, month, year):
    shipment_history_path = "../data/shipment_history/shipment_history.csv"
    days_shipments = get_shipment_at_date(day, month, year, shipment_history_path)
    output_stores_with_shipment(days_shipments)