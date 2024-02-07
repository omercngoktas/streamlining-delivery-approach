#include "../include/StoreManager.h"

StoreManager::StoreManager() {
    cout << "StoreManager created" << endl;
}

StoreManager::~StoreManager() {
    cout << "StoreManager destroyed" << endl;
}

void StoreManager::readStores(const string& filename) {
    ifstream file(filename);
    cout << "Reading stores from " << filename << endl;
    if (file.is_open()) {
        string line;
        printf("Reading stores from %s\n", filename.c_str());
        while (getline(file, line)) {
            stringstream ss(line);
            string id, location, address, depot_id, latitude, longitude;
            getline(ss, id, ',');
            getline(ss, location, ',');
            getline(ss, address, ',');
            getline(ss, depot_id, ',');
            getline(ss, latitude, ',');
            getline(ss, longitude, ',');
            Store store;
            store.store_id = id;
            store.store_location = location;
            store.store_address = address;
            store.depot_id = depot_id;
            store.latitude = stod(latitude);
            store.longitude = stod(longitude);
            stores.push_back(store);
        }
    }
}

void StoreManager::printStores() const {
    for (const auto& store : stores) {
        cout << "Store ID: " << store.store_id << ", Location: " << store.store_location << endl;
        // Print other store details as needed
    }
    cout << "Total stores: " << stores.size() << endl;
}