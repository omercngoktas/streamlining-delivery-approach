import os
import pandas as pd
from geopy.distance import geodesic

# Function to calculate the total distance of a given route
def calculate_total_distance(route):
    total_distance = 0.0
    for i in range(len(route) - 1):
        total_distance += geodesic(route[i], route[i+1]).kilometers
    return total_distance

# Nearest neighbor algorithm for route optimization
def nearest_neighbor(route_df):
    start_point = (route_df.iloc[0]['latitude'], route_df.iloc[0]['longitude'])
    points = [(row['latitude'], row['longitude']) for _, row in route_df.iterrows()]
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

# Function to process CSV files in a given folder
def process_folder(folder_path):
    csv_files = [f for f in os.listdir(folder_path) if f.endswith('.csv')]
    total_original_distance = 0.0
    total_optimized_distance = 0.0
    file_count = 0
    optimal_route_file = ""
    shortest_optimized_distance = float('inf')

    for csv_file in csv_files:
        file_path = os.path.join(folder_path, csv_file)
        df = pd.read_csv(file_path)

        routes = df['route'].unique()
        folder_optimized_distance = 0.0

        for route_number in routes:
            route_df = df[df['route'] == route_number]

            if not route_df.empty:
                # Calculate the distance before optimization
                original_route = [(row['latitude'], row['longitude']) for _, row in route_df.iterrows()]
                original_distance = calculate_total_distance(original_route)
                total_original_distance += original_distance

                # Optimize the route
                optimized_route = nearest_neighbor(route_df)
                optimized_distance = calculate_total_distance(optimized_route)
                total_optimized_distance += optimized_distance
                folder_optimized_distance += optimized_distance

        file_count += 1

        # Check if this file has the shortest optimized distance
        if folder_optimized_distance < shortest_optimized_distance:
            shortest_optimized_distance = folder_optimized_distance
            optimal_route_file = csv_file

    # Calculate average distances
    average_original_distance = total_original_distance / file_count if file_count > 0 else 0
    average_optimized_distance = total_optimized_distance / file_count if file_count > 0 else 0

    return average_original_distance, average_optimized_distance, optimal_route_file

# Main function to process all folders and save results
def main():
    base_folder_path = '/Users/omercangoktas/Desktop/github/routing-and-distribution-optimization/aco_wo_formula_new_version/output'  # Replace with the path to your base folder containing subfolders
    output_data = []

    for folder_name in os.listdir(base_folder_path):
        folder_path = os.path.join(base_folder_path, folder_name)
        if os.path.isdir(folder_path):
            avg_original_distance, avg_optimized_distance, optimal_route_file = process_folder(folder_path)
            output_data.append((folder_name, avg_original_distance, avg_optimized_distance, optimal_route_file))

    # Save the results to a CSV file
    results_df = pd.DataFrame(output_data, columns=['folder', 'average_original_distance', 'average_optimized_distance', 'optimal_route_file'])
    # sort results by average_optimized_distance
    results_df = results_df.sort_values(by='average_optimized_distance')
    results_df.to_csv('average_distances_with_optimal_file.csv', index=False)

if __name__ == "__main__":
    main()
