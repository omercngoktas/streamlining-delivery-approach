import pandas as pd
from geopy.distance import geodesic

# Load the CSV data into a DataFrame
df = pd.read_csv('routes.csv')

def calculate_total_distance(route):
    total_distance = 0.0
    for i in range(len(route) - 1):
        total_distance += geodesic(route[i], route[i+1]).kilometers
    return total_distance

def nearest_neighbor(route_df):
    start_point = (route_df.iloc[0]['latitude'], route_df.iloc[0]['longitude'])
    points = [(row['latitude'], row['longitude']) for idx, row in route_df.iterrows()]
    unvisited = points[1:-1]
    tour = [start_point]
    current_point = start_point

    while unvisited:
        next_point = min(unvisited, key=lambda point: geodesic(current_point, point).kilometers)
        tour.append(next_point)
        unvisited.remove(next_point)
        current_point = next_point

    tour.append(start_point)  # Return to the depot
    return tour

# Get unique route numbers
routes = df['route'].unique()
total_original_distance = 0.0
total_optimized_distance = 0.0

# Print all original routes and calculate the total distances before optimization
print("Original Routes and Distances Before Optimization:\n")
for route_number in routes:
    route_df = df[df['route'] == route_number]
    
    if not route_df.empty:
        # Calculate the distance before optimization
        original_route = [(row['latitude'], row['longitude']) for idx, row in route_df.iterrows()]
        original_distance = calculate_total_distance(original_route)
        total_original_distance += original_distance

        # Print the original route
        print(f"Original Route for Route {route_number}:", end=' ')
        for point in original_route:
            print(f"({point[0]}, {point[1]})", end=' ')
        print(f"\nDistance for Route {route_number} before optimization: {original_distance:.2f} kilometers\n")
    else:
        print(f"No data found for Route {route_number}.")

# Print a separator
print("="*100 + "\n")

# Save the optimized routes to a CSV file
optimized_route_data = []

# Print all optimized routes and calculate the total distances after optimization
print("Optimized Routes and Distances After Optimization:\n")
for route_number in routes:
    route_df = df[df['route'] == route_number]
    
    if not route_df.empty:
        # Optimize the route
        optimized_route = nearest_neighbor(route_df)
        optimized_distance = calculate_total_distance(optimized_route)
        total_optimized_distance += optimized_distance

        # Print the optimized route
        print(f"Optimized Route for Route {route_number}:", end=' ')
        for point in optimized_route:
            print(f"({point[0]}, {point[1]})", end=' ')
        print(f"\nDistance for Route {route_number} after optimization: {optimized_distance:.2f} kilometers\n")

        # Add the optimized route to the list for saving
        for i, point in enumerate(optimized_route):
            if i != 0 and i != len(optimized_route) - 1:
                optimized_route_data.append((route_number, point[0], point[1]))
    else:
        print(f"No data found for Route {route_number}.")

# Save the optimized routes to a CSV file
optimized_routes_df = pd.DataFrame(optimized_route_data, columns=['route', 'latitude', 'longitude'])

# add depot to the optimized route
optimized_route_data = []
for route_number in routes:
    route_df = df[df['route'] == route_number]
    if not route_df.empty:
        optimized_route = nearest_neighbor(route_df)
        for i, point in enumerate(optimized_route):
            optimized_route_data.append((route_number, point[0], point[1]))
            
optimized_routes_df = pd.DataFrame(optimized_route_data, columns=['route', 'latitude', 'longitude'])
optimized_routes_df.to_csv('optimized_routes.csv', index=False)

# Print total distances
print("="*100 + "\n")
print(f"Total distance for all routes before optimization: {total_original_distance:.2f} kilometers")
print(f"Total distance for all routes after optimization: {total_optimized_distance:.2f} kilometers")

total_distance_to_file = pd.DataFrame({'total_distance': [total_optimized_distance]})
# save the total distance to a file
total_distance_to_file.to_csv('total_distance.csv', index=False)