import requests

def fetch_duration_and_distance(api_key, origin_latitude, origin_longitude, destination_latitude, destination_longitude):
    base_url = 'https://maps.googleapis.com/maps/api/distancematrix/json?'
    # modes can be transit, driving, walking, bicycling
    travel_mode = 'driving'
    
    url = f'{base_url}origins={origin_latitude},{origin_longitude}&destinations={destination_latitude},{destination_longitude}&mode={travel_mode}&key={api_key}'

    response = requests.get(url)
    data = response.json()
    
    if 'rows' in data and len(data['rows']) > 0 and 'elements' in data['rows'][0] and len(data['rows'][0]['elements']) > 0:
        # Extract travel time from the response
        travel_time_text = data['rows'][0]['elements'][0].get('duration', {}).get('text', 'Unknown')
        distance_text = data['rows'][0]['elements'][0].get('distance', {}).get('text', 'Unknown')
        return travel_time_text, distance_text
    else:
        print('Error: Unable to calculate travel time.')
        return 'Unknown', 'Unknown'
