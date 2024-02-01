from selenium_fetcher import fetch_duration_and_distance
from get_location_details import get_depot, get_stores
import pandas as pd
from time import sleep
import multiprocessing

def fetch_duration_and_distance_wrapper(store1, store2):
    travel_time, distance = fetch_duration_and_distance(store1['latitude'], store1['longitude'], store2['latitude'], store2['longitude'])
    return {
        'current_id': store1['store_id'],
        'current_latitude': store1['latitude'],
        'current_longitude': store1['longitude'],
        'duration': travel_time,
        'distance': distance,
        'next_id': store2['store_id'],
        'next_latitude': store2['latitude'],
        'next_longitude': store2['longitude']
    }

def main():
    stores = get_stores()
    depot = get_depot()
    # create a dataframe which contains the store details and duration and distance from this store to next store
    # also contains the detail of this next store
    output = pd.DataFrame(columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    
    # # Create a pool of processes
    # pool = multiprocessing.Pool(processes=multiprocessing.cpu_count())  # Use number of CPU cores for parallel processing
    
    # iterate through all the stores
    for i in range(len(stores)):
        for j in range(len(stores)):
            if i != j:
                travel_time, distance = fetch_duration_and_distance(stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude'])
                # output = output.append({'current_id': stores.iloc[i]['store_id'], 'current_latitude': stores.iloc[i]['latitude'], 'current_longitude': stores.iloc[i]['longitude'], 'duration': travel_time, 'distance': distance, 'next_id': stores.iloc[j]['store_id'], 'next_latitude': stores.iloc[j]['latitude'], 'next_longitude': stores.iloc[j]['longitude']}, ignore_index=True)
                new_output = pd.DataFrame([[stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], travel_time, distance, stores.iloc[j]['store_id'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
                # adding new row to the output dataframe with using concat
                output = pd.concat([output, new_output], ignore_index=True)
                output.to_csv('./output.csv', index=False)
            
if __name__ == "__main__":
    main()