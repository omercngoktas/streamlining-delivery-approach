import os
import pandas as pd
import folium
import osmnx as ox
import networkx as nx
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
import time
import sys

output_name = sys.argv[1]
csv_file = sys.argv[2]

date = time.strftime("%Y-%m-%d")

# Read optimized routes CSV file
try:
    df = pd.read_csv(csv_file, sep=',', skiprows=1, names=['route', 'latitude', 'longitude'])
    print("CSV dosyası başarıyla okundu.")
except FileNotFoundError:
    print(f"Dosya bulunamadı: {csv_file}")
    exit()
except Exception as e:
    print(f"Bir hata oluştu: {e}")
    exit()

# Convert latitude and longitude columns to float
try:
    df['latitude'] = df['latitude'].astype(float)
    df['longitude'] = df['longitude'].astype(float)
    print("Veriler başarıyla dönüştürüldü.")
except Exception as e:
    print(f"Verileri işlerken bir hata oluştu: {e}")
    exit()

# Calculate average latitude and longitude for initial map center
average_lat = df['latitude'].mean()
average_lon = df['longitude'].mean()
initial_location = [average_lat, average_lon]

# Define color list for routes
colors = [
    'blue', 'green', 'red', 'purple', 'orange', 'darkred', 'darkblue', 'darkgreen',
    'black', 'cadetblue', 'pink', 'lightblue', 'lightgreen',
    'lightgray', 'lightred', 'beige', 'darkbrown', 'darkpink', 'gold', 'lightorange',
    'darkyellow', 'lightpurple'
]

# Find unique routes
routes = df['route'].unique()

# Download OSM road network (expand the scope)
G = ox.graph_from_point((average_lat, average_lon), dist=20000, network_type='all')

# Initialize Selenium WebDriver once
service = Service(ChromeDriverManager().install())
options = webdriver.ChromeOptions()
options.add_argument('--headless')
options.add_argument('--no-sandbox')
options.add_argument('--disable-dev-shm-usage')
driver = webdriver.Chrome(service=service, options=options)

# List to store all coordinates for the combined map
all_coordinates = []

for route in routes:
    route_df = df[df['route'] == route]
    initial_location = [route_df.iloc[0]['latitude'], route_df.iloc[0]['longitude']]
    color = colors[int(route) % len(colors)]  # Determine color for this route

    # Create a new map for the route
    route_map = folium.Map(location=initial_location, zoom_start=12)

    # Add each coordinate to the map with the same color
    for index, row in route_df.iterrows():
        folium.Marker([row['latitude'], row['longitude']], popup=f"Route {row['route']} - Point {index+1}", icon=folium.Icon(color=color)).add_to(route_map)

    # Get coordinates of the route and draw line
    route_coordinates = route_df[['latitude', 'longitude']].values.tolist()
    for i in range(len(route_coordinates) - 1):
        orig = ox.distance.nearest_nodes(G, X=route_coordinates[i][1], Y=route_coordinates[i][0])
        dest = ox.distance.nearest_nodes(G, X=route_coordinates[i+1][1], Y=route_coordinates[i+1][0])
        route_path = nx.shortest_path(G, orig, dest, weight='length')
        route_latlongs = [(G.nodes[n]['y'], G.nodes[n]['x']) for n in route_path]
        folium.PolyLine(route_latlongs, color=color, weight=2.5, opacity=1).add_to(route_map)

        # Add coordinates to the list for the combined map
        all_coordinates.extend(route_latlongs)

    # Define temporary file name and path for the route map
    route_html_file = f"{output_name}_{date}_route_{route}_map.html"
    route_html_path = os.path.abspath(route_html_file)

    # Save the route map to HTML file
    route_map.save(route_html_path)

    # # Open the route map
    # driver.get("file://" + route_html_path)

    # # Wait for the map to load
    # time.sleep(5)

    # # Take screenshot of the route map
    # screenshot_path = f"{output_name}_{date}_route_{route}_map_screenshot.png"
    # driver.save_screenshot(screenshot_path)
    # print(f"Ekran görüntüsü alındı ve {screenshot_path} dosyasına kaydedildi.")

# Now create the combined map with all routes
combined_map = folium.Map(location=initial_location, zoom_start=12)

for route in routes:
    route_df = df[df['route'] == route]
    color = colors[int(route) % len(colors)]  # Determine color for this route

    # Create a feature group for the route
    fg = folium.FeatureGroup(name=f"Route {route}")

    # Add each coordinate to the feature group
    for index, row in route_df.iterrows():
        folium.Marker([row['latitude'], row['longitude']], popup=f"Route {row['route']} - Point {index+1}", icon=folium.Icon(color=color)).add_to(fg)

    # Get coordinates of the route and draw line
    route_coordinates = route_df[['latitude', 'longitude']].values.tolist()
    for i in range(len(route_coordinates) - 1):
        orig = ox.distance.nearest_nodes(G, X=route_coordinates[i][1], Y=route_coordinates[i][0])
        dest = ox.distance.nearest_nodes(G, X=route_coordinates[i+1][1], Y=route_coordinates[i+1][0])
        route_path = nx.shortest_path(G, orig, dest, weight='length')
        route_latlongs = [(G.nodes[n]['y'], G.nodes[n]['x']) for n in route_path]
        folium.PolyLine(route_latlongs, color=color, weight=2.5, opacity=1).add_to(fg)

    # Add the feature group to the combined map
    fg.add_to(combined_map)

# Add layer control to the combined map
folium.LayerControl().add_to(combined_map)

# Set map bounds to include all coordinates
combined_map.fit_bounds(all_coordinates)

# Define file name and path for the combined map
combined_html_file = f"{output_name}_{date}_combined_map.html"
combined_html_path = os.path.abspath(combined_html_file)

# Save the combined map to HTML file
combined_map.save(combined_html_path)

# Open the combined map
driver.get("file://" + combined_html_path)

# Wait for the map to load
time.sleep(5)

# Take screenshot of the combined map
combined_screenshot_path = f"{output_name}_{date}_combined_map_screenshot.png"
driver.save_screenshot(combined_screenshot_path)
print(f"Ekran görüntüsü alındı ve {combined_screenshot_path} dosyasına kaydedildi.")

# Close the browser
driver.quit()

# Read total distance from another CSV file (if applicable)
total_distance_file = "total_distance.csv"
try:
    total_distance_df = pd.read_csv(total_distance_file)
    total_distance = total_distance_df.iloc[0]['total_distance']
    print(f"Total distance: {total_distance}")
except FileNotFoundError:
    print(f"Dosya bulunamadı: {total_distance_file}")
except Exception as e:
    print(f"Hata oluştu: {e}")
