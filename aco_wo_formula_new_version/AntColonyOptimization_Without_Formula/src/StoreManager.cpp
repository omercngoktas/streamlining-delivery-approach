#include "../include/StoreManager.h"

// Constructor for Store
Store::Store(const string& store_id, const string& store_location, const string& store_address, const string& depot_id, const double& latitude, const double& longitude)
    : store_id(store_id), store_location(store_location), store_address(store_address), depot_id(depot_id), latitude(latitude), longitude(longitude) {
}

// Add a store to the store manager
void StoreManager::addStore(const string& store_id, const string& store_location, const string& store_address, const string& depot_id, const double& latitude, const double& longitude) {
    auto store = std::make_unique<Store>(store_id, store_location, store_address, depot_id, latitude, longitude);
    stores.push_back(move(store));
}

// Display the stores
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

// Display the store
void Store::displayStore() const {
    cout << "Store ID: " << store_id;
    cout << " Store Location: " << store_location;
    cout << " Store Address: " << store_address;
    cout << " Depot ID: " << depot_id;
    cout << " Latitude: " << latitude;
    cout << " Longitude: " << longitude;
    cout << " Number of Pallets: " << num_of_pallets << endl;
}

// Removing a store from the given store vector with the given store_id
void StoreManager::removeStore(vector<Store>& stores, string store_id) const {
    stores.erase(
        std::remove_if(stores.begin(), stores.end(), 
            [&store_id](const Store& store) { 
                return store.getStoreId() == store_id; 
            }), 
        stores.end());
}

// Get a random store from the given vector
Store StoreManager::getRandStore(vector<Store>& currentStores) {
    int randomIndex = rand() % currentStores.size();
    return currentStores[randomIndex];
}

// Get the store by store_id
Store StoreManager::getStoreById(const string& store_id) const {
    for (const auto& store : stores) {
        if (store->getStoreId() == store_id) {
            return *store;
        }
    }
    throw invalid_argument("Store ID not found");
}

// get distance between store and store
double Store::getDistanceToStore(double storeLatitude, double storeLongitude) {
    double distance = sqrt(pow(storeLatitude - latitude, 2) + pow(storeLongitude - longitude, 2));
    return distance;
}