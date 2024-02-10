import geopy.distance

coords_1 = (40.8833157,29.3681184)
coords_2 = (40.9869642,29.1319157)

print(geopy.distance.geodesic(coords_1, coords_2).km)

# import googlemaps

# # API anahtarınızı ekleyin
# gmaps = googlemaps.Client(key='AIzaSyBrHncB-iJ-9fksC_464fT8MGZLzTkndqQ')

# # Başlangıç ve varış noktalarının koordinatlarını belirtin
# start_location = (40.8833157, 29.3681184)  # Örnek başlangıç noktası koordinatları
# end_location = (40.9869642, 29.1319157)    # Örnek varış noktası koordinatları

# # Distance Matrix API'sini kullanarak mesafeyi ve süreyi hesaplayın
# result = gmaps.distance_matrix(start_location, end_location, mode="driving", units="metric")

# # Mesafeyi ve süreyi alın
# distance = result['rows'][0]['elements'][0]['distance']['text']
# duration = result['rows'][0]['elements'][0]['duration']['text']

# # Sonuçları yazdırın
# print("Mesafe: ", distance)
# print("Süre: ", duration)
