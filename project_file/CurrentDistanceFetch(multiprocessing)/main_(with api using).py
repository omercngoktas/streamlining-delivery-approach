from get_location_details import get_stores, get_depot
from distance_time_find import fetch_duration_and_distance
import pandas as pd
from time import sleep

def main():
    api_key = 'AIzaSyBkzwqzGynUriSWTVsOaSC70eUy2zoMyv0'
    stores = get_stores()
    depot = get_depot()
    # create a dataframe which contains the store details and duration and distance from this store to next store
    # also contains the detail of this next store
    output = pd.DataFrame(columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
    
    # find the duration and distance from current store to next store
    # and add the next store details to the dataframe, then repeat this process for all stores
    # using two for loops
    for i in range(len(stores)):
        for j in range(len(stores)):
            if i != j:
                travel_time, distance = fetch_duration_and_distance(api_key, stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude'])
                # output = output.append({'current_id': stores.iloc[i]['store_id'], 'current_latitude': stores.iloc[i]['latitude'], 'current_longitude': stores.iloc[i]['longitude'], 'duration': travel_time, 'distance': distance, 'next_id': stores.iloc[j]['store_id'], 'next_latitude': stores.iloc[j]['latitude'], 'next_longitude': stores.iloc[j]['longitude']}, ignore_index=True)
                new_output = pd.DataFrame([[stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], travel_time, distance, stores.iloc[j]['store_id'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude']]], columns=['current_id', 'current_latitude', 'current_longitude', 'duration', 'distance', 'next_id', 'next_latitude', 'next_longitude'])
                # adding new row to the output dataframe with using concat
                output = pd.concat([output, new_output], ignore_index=True)
                output.to_csv('./output.csv', index=False)
            # sleep for 0.5 seconds to avoid OVER_QUERY_LIMIT error
            sleep(0.5)


    

if __name__ == '__main__':
    main()
