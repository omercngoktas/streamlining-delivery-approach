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
    unvisited = points[1:]
    tour = []
    current_point = start_point

    while unvisited:
        next_point = min(unvisited, key=lambda point: geodesic(current_point, point).kilometers)
        tour.append(next_point)
        unvisited.remove(next_point)
        current_point = next_point

    tour.append(start_point)  # Return to the depot
    return tour

def is_valid_line(line):
    try:
        float(line.split(' ')[-1])
        return True
    except ValueError:
        return False

# process_folder function will open csv files under subfolder of given path
# then it will calculate the total distance of the original route and optimized route
# then it will print the total distance of the original route and optimized route
def process_folder(folder_path):
    # read subfolders of the given path
    subfolders = [f.path for f in os.scandir(folder_path) if f.is_dir()]
    
    output_text = ""
    
    for subfolder in subfolders:
        # read csv files under the subfolder
        csv_files = [f for f in os.listdir(subfolder) if f.endswith('.csv')]
        
        best_distance = float('inf')
        best_route = []
        average_distance = 0.0
        best_route_folder = ""
        
        for csv_file in csv_files:
            file_path = os.path.join(subfolder, csv_file)
            df = pd.read_csv(file_path)

            routes = df['route'].unique()
            folder_optimized_distance = 0.0
            folder_optimized_route = []
            
            

            for route_number in routes:
                route_df = df[df['route'] == route_number]

                if not route_df.empty:
                    # Calculate the distance before optimization
                    original_route = [(row['latitude'], row['longitude']) for _, row in route_df.iterrows()]
                    original_distance = calculate_total_distance(original_route)
                    # Optimize the route
                    optimized_route = nearest_neighbor(route_df)
                    optimized_distance = calculate_total_distance(optimized_route)
                    
                        
                    folder_optimized_distance += optimized_distance
                    folder_optimized_route += route_df.to_dict('records')
                    average_distance += optimized_distance
                    
            if folder_optimized_distance < best_distance:
                best_distance = folder_optimized_distance
                best_route = folder_optimized_route
                best_route_folder = subfolder.split('/')[-1]
        
        subfolder = subfolder.split('/')[-1]
        output_text += f"Average distance for '{subfolder}' is {average_distance / len(csv_files)}\n"
    
    # sort output text by average distance
    lines = [line for line in output_text.split('\n') if line.strip() and is_valid_line(line)]

    output_text = sorted(lines, key=lambda x: float(x.split(' ')[-1]))    
    # join the sorted lines back into a single string
    output_text = '\n'.join(output_text)
    
    output_text += f"\nBest route is in folder '{best_route_folder}' with distance {best_distance}\n"
    # save the output text to a file
    with open('output.txt', 'w') as f:
        f.write(output_text)
    
    # best route to csv file
    df = pd.DataFrame(best_route)
    df.to_csv(f'best_route.csv', index=False)
    
        
        
    
            
    
    
    
def main():
    base_folder_path = '/Users/omercangoktas/Desktop/github/routing-and-distribution-optimization/aco_wo_formula_new_version/output' 
    process_folder(base_folder_path)
    
    
    
    
    
if __name__ == '__main__':
    main()
