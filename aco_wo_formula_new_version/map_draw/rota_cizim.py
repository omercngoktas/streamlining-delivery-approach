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

date = today = time.strftime("%Y-%m-%d")

# Read optimized routes CSV file
csv_file = "./optimized_routes.csv"
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

# Create map with initial center
mymap = folium.Map(location=initial_location, zoom_start=12)

# Define color list for routes
colors = ['blue', 'green', 'red', 'purple', 'orange', 'darkred', 'darkblue', 'darkgreen','black',  'darkpurple', 'gray']

# Find unique routes
routes = df['route'].unique()

# Download OSM road network (expand the scope)
G = ox.graph_from_point((average_lat, average_lon), dist=20000, network_type='all')

# List to store all coordinates
all_coordinates = []

# Create a map for each route
for route in routes:
    route_df = df[df['route'] == route]
    initial_location = [route_df.iloc[0]['latitude'], route_df.iloc[0]['longitude']]
    color = colors[int(route) % len(colors)] # Determine color for this route
    # Add each coordinate to the map with the same color
    for index, row in route_df.iterrows():
        # folium.Marker([row['latitude'], row['longitude']], popup=f"Route {row['route']} - Point {index+1}", icon=folium.Icon(color='blue')).add_to(mymap)
        folium.Marker([row['latitude'], row['longitude']], popup=f"Route {row['route']} - Point {index+1}", icon=folium.Icon(color=color)).add_to(mymap)

        all_coordinates.append([row['latitude'], row['longitude']])

    # Get coordinates of the route and draw line
    route_coordinates = route_df[['latitude', 'longitude']].values.tolist()
    for i in range(len(route_coordinates) - 1):
        orig = ox.distance.nearest_nodes(G, X=route_coordinates[i][1], Y=route_coordinates[i][0])
        dest = ox.distance.nearest_nodes(G, X=route_coordinates[i+1][1], Y=route_coordinates[i+1][0])
        route_path = nx.shortest_path(G, orig, dest, weight='length')
        route_latlongs = [(G.nodes[n]['y'], G.nodes[n]['x']) for n in route_path]
        folium.PolyLine(route_latlongs, color=color, weight=2.5, opacity=1).add_to(mymap)


# Read total distance from another CSV file
total_distance_file = "total_distance.csv"
try:
    total_distance_df = pd.read_csv(total_distance_file)
    total_distance = total_distance_df.iloc[0]['total_distance']
    print(f"Total distance: {total_distance}")
    # Add total distance as a marker to the map
    folium.Marker(initial_location, popup=f"Total Distance: {total_distance}").add_to(mymap)
except FileNotFoundError:
    print(f"Dosya bulunamadı: {total_distance_file}")
except Exception as e:
    print(f"Hata oluştu: {e}")

# Set map bounds to include all coordinates
mymap.fit_bounds(all_coordinates)

# Define temporary file name and path
html_file = f"{output_name}_{date}_map.html"
html_path = os.path.abspath(html_file)

# Save map to HTML file
mymap.save(html_path)

# Open map in web browser using Selenium and take screenshot
service = Service(ChromeDriverManager().install())
options = webdriver.ChromeOptions()
options.add_argument('--headless')
options.add_argument('--no-sandbox')
options.add_argument('--disable-dev-shm-usage')
driver = webdriver.Chrome(service=service, options=options)

# Open the map
driver.get("file://" + html_path)

# Wait for the map to load
time.sleep(5)

# Take screenshot and save
screenshot_path = f"{output_name}_{date}_map_screenshot.png"
driver.save_screenshot(screenshot_path)

# Close the browser
driver.quit()

print(f"Ekran görüntüsü alındı ve {screenshot_path} dosyasına kaydedildi.")
