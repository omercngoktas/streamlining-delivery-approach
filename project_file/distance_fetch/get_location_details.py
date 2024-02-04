import pandas as pd

def get_stores(file_path='../data/store/stores.csv'):
    stores = pd.read_csv(file_path)
    return stores

def get_depot():
    file_path = '../data/depot/depots.csv'  # Replace 'your_file.csv' with the path to your CSV file
    depot = pd.read_csv(file_path)
    return depot