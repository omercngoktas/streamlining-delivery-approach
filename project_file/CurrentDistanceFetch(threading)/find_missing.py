
def find_missing_results(stores, results):
    
    for i in range(len(stores)):
        for j in range(len(stores)):
            if i != j:
                if not any(result[0] == stores.iloc[i]['store_id'] and result[5] == stores.iloc[j]['store_id'] for result in results):
                    print('Missing: ', stores.iloc[i]['store_id'], stores.iloc[j]['store_id'])
                    # travel_time, distance = fetch_duration_and_distance(stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude'])
                    # results.append([stores.iloc[i]['store_id'], stores.iloc[i]['latitude'], stores.iloc[i]['longitude'], travel_time, distance, stores.iloc[j]['store_id'], stores.iloc[j]['latitude'], stores.iloc[j]['longitude'])
                    # print('Processed: ', i, j)