#include "../include/StoreManager.h"

// Parameterized constructor
Store::Store(const string& store_id, const string& store_location, const string& store_address, const string& depot_id, const double& latitude, const double& longitude)
    : store_id(store_id), store_location(store_location), store_address(store_address), depot_id(depot_id), latitude(latitude), longitude(longitude) {
}

// Getters
string Store::getStoreId() const { return store_id; }
string Store::getStoreLocation() const { return store_location; }
string Store::getStoreAddress() const { return store_address; }
string Store::getDepotId() const { return depot_id; }
double Store::getLatitude() const { return latitude; }
double Store::getLongitude() const { return longitude; }
// Setters
void Store::setStoreId(const string& store_id) { this->store_id = store_id; }
void Store::setStoreLocation(const string& store_location) { this->store_location = store_location; }
void Store::setStoreAddress(const string& store_address) { this->store_address = store_address; }
void Store::setDepotId(const string& depot_id) { this->depot_id = depot_id; }
void Store::setLatitude(const double& latitude) { this->latitude = latitude; }
void Store::setLongitude(const double& longitude) { this->longitude = longitude; }

void StoreManager::addStore(const string& store_id, const string& store_location, const string& store_address, const string& depot_id, const double& latitude, const double& longitude) {
    auto store = std::make_unique<Store>(store_id, store_location, store_address, depot_id, latitude, longitude);
    stores.push_back(move(store));
}

void StoreManager::displayStores() {
    for (const auto& store : stores) {
        cout << "Store ID: " << store->getStoreId() << "\t";
        cout << "Store Location: " << store->getStoreLocation() << "\t\t";
        cout << "Store Address: " << store->getStoreAddress() << "\t";
        cout << "Depot ID: " << store->getDepotId() << "\t";
        cout << "Latitude: " << store->getLatitude() << "\t";
        cout << "Longitude: " << store->getLongitude() << endl;

    }
}