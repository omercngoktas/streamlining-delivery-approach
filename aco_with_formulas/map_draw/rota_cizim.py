import os
import pandas as pd
import folium
import osmnx as ox
import networkx as nx
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
import time

# CSV dosyasını okuma ve sütun isimlerini belirleme (ilk satırı atlayarak)
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

# Verileri dönüştürme
try:
    df['latitude'] = df['latitude'].astype(float)
    df['longitude'] = df['longitude'].astype(float)
    print("Veriler başarıyla dönüştürüldü.")
except Exception as e:
    print(f"Verileri işlerken bir hata oluştu: {e}")
    exit()

# Haritanın merkezini belirlemek için tüm konumların ortalamasını hesaplama
average_lat = df['latitude'].mean()
average_lon = df['longitude'].mean()
initial_location = [average_lat, average_lon]

# Harita oluşturma (merkezi belirli bir konum olarak ayarlayarak)
mymap = folium.Map(location=initial_location, zoom_start=12)

# Renk listesi
colors = ['blue', 'green', 'red', 'purple', 'orange', 'darkred', 'lightred', 'beige', 'darkblue', 'darkgreen', 'cadetblue', 'darkpurple', 'pink', 'lightblue', 'lightgreen', 'gray', 'black', 'lightgray']

# Benzersiz rotaları bulma
routes = df['route'].unique()

# OSM yol ağını indirme (kapsamı genişletme)
G = ox.graph_from_point((average_lat, average_lon), dist=20000, network_type='all')

# Tüm koordinatları toplamak için bir liste
all_coordinates = []

# Her rota için ayrı bir harita oluşturma
for route in routes:
    route_df = df[df['route'] == route]
    initial_location = [route_df.iloc[0]['latitude'], route_df.iloc[0]['longitude']]
    
    # Her bir koordinatı haritaya ekleme
    for index, row in route_df.iterrows():
        folium.Marker([row['latitude'], row['longitude']], popup=f"Route {row['route']} - Point {index+1}").add_to(mymap)
        all_coordinates.append([row['latitude'], row['longitude']])

    # Rotanın koordinatlarını alıp çizgi çekme (yollar boyunca)
    route_coordinates = route_df[['latitude', 'longitude']].values.tolist()
    for i in range(len(route_coordinates) - 1):
        orig = ox.distance.nearest_nodes(G, X=route_coordinates[i][1], Y=route_coordinates[i][0])
        dest = ox.distance.nearest_nodes(G, X=route_coordinates[i+1][1], Y=route_coordinates[i+1][0])
        route_path = nx.shortest_path(G, orig, dest, weight='length')
        route_latlongs = [(G.nodes[n]['y'], G.nodes[n]['x']) for n in route_path]
        folium.PolyLine(route_latlongs, color=colors[int(route) % len(colors)], weight=2.5, opacity=1).add_to(mymap)

# Tüm koordinatları kapsayacak şekilde harita sınırlarını ayarlama
mymap.fit_bounds(all_coordinates)

# Geçici dosya adı ve tam yolu belirleme
html_file = "map.html"
html_path = os.path.abspath(html_file)

# Haritayı HTML dosyasına kaydetme
mymap.save(html_path)

# Selenium kullanarak web tarayıcısında haritayı açma ve ekran görüntüsü alma
service = Service(ChromeDriverManager().install())
options = webdriver.ChromeOptions()
options.add_argument('--headless')  # Tarayıcıyı arka planda çalıştırmak için
options.add_argument('--no-sandbox')
options.add_argument('--disable-dev-shm-usage')
driver = webdriver.Chrome(service=service, options=options)

# Haritayı aç
driver.get("file://" + html_path)

# Haritanın yüklenmesi için bekleme süresi
time.sleep(5)

# Ekran görüntüsü al ve kaydet
screenshot_path = "map_screenshot.png"
driver.save_screenshot(screenshot_path)

# Tarayıcıyı kapat
driver.quit()

print(f"Ekran görüntüsü alındı ve {screenshot_path} dosyasına kaydedildi.")
