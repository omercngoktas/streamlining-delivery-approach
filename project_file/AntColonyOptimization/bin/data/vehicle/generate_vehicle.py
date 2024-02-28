import random
import pandas as pd

def generate_vehicle_id(length=20):
    letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    numbers = "0123456789"
    return ''.join(random.choice(letters + numbers) for _ in range(length))

# Araç ID'leri oluştur
vehicle_ids = ["VH-" + generate_vehicle_id() for _ in range(10)]

# Bağlı Depo ID
depot_id = "DP-9NCBLLHI6RU2U7SXDNRH"

# Palet kapasiteleri
palette_capacities = [random.choice([15, 18, 9]) for _ in range(10)]

# Araç durumları
statuses = ["Available"] * 10

# Yakıt tüketimi (lt/100 km)
fuel_consumptions = [random.randint(12, 24) - (15 - palette_capacities[i]) for i in range(10)]

# Veri setini oluştur
data = {
    "vehicle_id": vehicle_ids,
    "depot_id": [depot_id] * 10,
    "palette_capacity": palette_capacities,
    "status": statuses,
    "fuel_consumption(lt/100km)": fuel_consumptions
}

# Veriyi bir DataFrame'e dönüştür
df = pd.DataFrame(data)

# Veriyi CSV dosyasına kaydet
df.to_csv("vehicles.csv", index=False)
