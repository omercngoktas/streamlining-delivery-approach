import pandas as pd
import folium
import osmnx as ox
import networkx as nx

# Read data from CSV files
df = pd.read_csv('optimized_routes.csv')
total_distance_df = pd.read_csv('total_distance.csv')

# Extract total distance
total_distance = total_distance_df.iloc[0]['total_distance']

# Create a base map
m = folium.Map(location=[40.8833, 29.3681], zoom_start=10)

# Download the road network graph from OpenStreetMap using bbox
G = ox.graph_from_bbox(north=41.15, south=40.85, east=29.4, west=29.0, network_type='drive')

# Get unique routes
routes = df['route'].unique()

# Define colors for the routes
colors = ['blue', 'green', 'red', 'purple', 'orange', 'darkred', 'darkblue', 'darkgreen', 'darkpurple', 'pink', 'gray', 'black']

# Map routes to colors
route_color_map = dict(zip(routes, colors))

# Plot each route
for route in routes:
    points = df[df['route'] == route]
    route_points = list(zip(points.latitude, points.longitude))
    route_coords = []
    
    for j in range(len(route_points) - 1):
        start = ox.distance.nearest_nodes(G, points.longitude.iloc[j], points.latitude.iloc[j])
        end = ox.distance.nearest_nodes(G, points.longitude.iloc[j + 1], points.latitude.iloc[j + 1])
        
        # Find the shortest path
        shortest_route = nx.shortest_path(G, start, end, weight='length')
        
        # Convert node list to coordinate list
        route_coords += [(G.nodes[node]['y'], G.nodes[node]['x']) for node in shortest_route]
    
    folium.PolyLine(
        locations=route_coords,
        color=route_color_map[route],
        weight=5,
        opacity=0.7
    ).add_to(m)

# Add points as markers with different colors
for idx, row in df.iterrows():
    route = row['route']
    marker_color = route_color_map[route]
    # Check if this is the depot
    if idx == 0:  # Assuming the first point is the depot
        folium.Marker(
            location=[row.latitude, row.longitude],
            icon=folium.Icon(color=marker_color, icon='home', prefix='fa'),
            popup=f"Depot\nTotal Distance: {total_distance:.2f} km"
        ).add_to(m)
    else:
        folium.Marker(
            location=[row.latitude, row.longitude],
            icon=folium.Icon(color=marker_color, icon='info-sign'),
            popup=f"({row.latitude}, {row.longitude})"
        ).add_to(m)

# Save to an HTML file
m.save('real_routes_map.html')
