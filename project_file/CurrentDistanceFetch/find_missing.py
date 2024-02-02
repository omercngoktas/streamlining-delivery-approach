
def find_missing_results(stores, results):
    
    print(results)
    
    lat1 = 40.9789263
    lon1 = 29.117322
    lat2 = 40.982326
    lon2 = 29.132531
    
    
    for i in range(len(stores)):
        if (lat1 in results[i] and lon1 in results[i]) and (lat2 in results[i] and lon2 in results[i]):
            print('found:', results[i])