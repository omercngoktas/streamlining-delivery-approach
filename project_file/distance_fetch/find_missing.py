
def find_missing_results(stores, results):
    missing_results = []
    for i in range(len(stores)):
        for j in range(len(stores)):
            if i != j:
                if not any(result[0] == stores.iloc[i]['store_id'] and result[5] == stores.iloc[j]['store_id'] for result in results):
                    missing_results.append([stores.iloc[i]['store_id'], stores.iloc[j]['store_id']])
        
    return missing_results