import pandas as pd

from data.shipment_history.generate_shipment_history import generate_history_till_today

class ShipmentManager:
    def __init__(self, shipment_history_path, output_path):
        self.shipment_history_path = shipment_history_path
        self.shipment_history = pd.read_csv(shipment_history_path)
        self.output_path = output_path
        generate_history_till_today()
    
    def get_shipment_at_date(self, day, month, year):
        self.shipment_history["date"] = pd.to_datetime(self.shipment_history["date"])
        return self.shipment_history[(self.shipment_history["date"].dt.day == day) & (self.shipment_history["date"].dt.month == month) & (self.shipment_history["date"].dt.year == year)]
    
    def output_stores_with_shipment(self, shipments):
        stores = []
        
        for index, row in shipments.iterrows():
            if row["shipments"] > 0:
                stores.append(row["store_id"])
        
        output_df = pd.DataFrame(stores, columns=["store_id"])
        output_df.to_csv(self.output_path, index=False)
    
    def given_days_shipment_to_file(self, day, month, year):
        days_shipments = self.get_shipment_at_date(day, month, year)
        # self.output_stores_with_shipment(days_shipments)
        
    def insert_new_shipment_history(self):
        new_shipment_history = pd.read_csv("./data/shipment_history/new_shipment_history.csv")
        return new_shipment_history