import requests

# Google Haritalar Yönlendirme API'si URL'si
directions_url = "https://maps.googleapis.com/maps/api/directions/json"

start_location = {
    "lat": 40.8833157,
    "lng": 29.3681184
}

end_location = {
    "lat": 40.9184004,
    "lng": 29.2204548
}

# İki konum arasındaki yol bilgilerini almak için API anahtarınızı kullanın
params = {
    "origin": f"{start_location['lat']},{start_location['lng']}",
    "destination": f"{end_location['lat']},{end_location['lng']}",
    "key": "AIzaSyBrHncB-iJ-9fksC_464fT8MGZLzTkndqQ"
}


response = requests.get(directions_url, params=params)
data = response.json()

# Yol bilgilerini alın
path_coordinates = []
for step in data["routes"][0]["legs"][0]["steps"]:
    start_location = step["start_location"]
    path_coordinates.append((start_location["lat"], start_location["lng"]))

print(path_coordinates)

import folium

# Yol koordinatlarını içeren path_coordinates listesini kullanarak bir harita oluşturun
m = folium.Map(location=path_coordinates[0], zoom_start=13)

# İki konumu işaretleyin
folium.Marker(location=path_coordinates[0], tooltip="Başlangıç").add_to(m)
folium.Marker(location=path_coordinates[-1], tooltip="Bitiş").add_to(m)

# Yolu çizin
folium.PolyLine(locations=path_coordinates, color="blue", weight=5).add_to(m)

# Haritayı kaydedin
m.save("yol_haritasi.html")
