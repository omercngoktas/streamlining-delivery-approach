import pandas as pd

class LocationManager:
    def __init__(self, stores_path, depot_path):
        self.stores_path = stores_path
        self.depot_path = depot_path
        
    def get_stores(self, stores_path=None):
        if stores_path:
            stores = pd.read_csv(stores_path)
        else:
            stores = pd.read_csv(self.stores_path)
        return stores
    
    def get_depot(self):
        depot = pd.read_csv(self.depot_path)
        return depot
