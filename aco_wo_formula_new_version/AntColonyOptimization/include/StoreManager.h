#ifndef STORE_MANAGER
#define STORE_MANAGER

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <memory>

using namespace std;

class Store {
    public:
        Store() = default;
        Store(const string& store_id, const string& store_location, const string& store_address, const string& depot_id, const double& latitude, const double& longitude);
        ~Store() = default;

        // Getters
        string getStoreId() const { return store_id; }
        string getStoreLocation() const { return store_location; }
        string getStoreAddress() const { return store_address; }
        string getDepotId() const { return depot_id; }
        double getLatitude() const { return latitude; }
        double getLongitude() const { return longitude; }
        int getNumOfPallets() const { return num_of_pallets; }

        // Setters
        void setStoreId(const string& store_id) { this->store_id = store_id; }
        void setStoreLocation(const string& store_location) { this->store_location = store_location; }
        void setStoreAddress(const string& store_address) { this->store_address = store_address; }
        void setDepotId(const string& depot_id) { this->depot_id = depot_id; }
        void setLatitude(const double& latitude) { this->latitude = latitude; }
        void setLongitude(const double& longitude) { this->longitude = longitude; }
        void setNumOfPallets(const int& num_of_pallets) { this->num_of_pallets = num_of_pallets; }

        void displayStore() const;

    private:
        string store_id;
        string store_location;
        string store_address;
        string depot_id;
        double latitude;
        double longitude;
        int num_of_pallets;
};

class StoreManager {
    public:
        StoreManager() = default;
        ~StoreManager() = default;
        void addStore(const string& store_id, const string& store_location, const string& store_address, const string& depot_id, const double& latitude, const double& longitude);
        void displayStores();
        int getNumOfStores() const;
        void removeStore(vector<Store>& stores, string store_id) const;
        Store getRandStore(vector<Store>& currentStores);
        Store getStoreById(const string& store_id) const;
        const vector<unique_ptr<Store>>& getStores() const { return stores; }
        
    private:
        vector<unique_ptr<Store>> stores;

};

#endif