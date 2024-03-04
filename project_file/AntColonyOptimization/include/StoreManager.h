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

        // Setters
        void setStoreId(const string& store_id);
        void setStoreLocation(const string& store_location);
        void setStoreAddress(const string& store_address);
        void setDepotId(const string& depot_id);
        void setLatitude(const double& latitude);
        void setLongitude(const double& longitude);

        // Getters
        string getStoreId() const;
        string getStoreLocation() const;
        string getStoreAddress() const;
        string getDepotId() const;
        double getLatitude() const;
        double getLongitude() const;

    private:
        string store_id;
        string store_location;
        string store_address;
        string depot_id;
        double latitude;
        double longitude;
};

class StoreManager {
    public:
        StoreManager() = default;
        ~StoreManager() = default;
        void addStore(const string& store_id, const string& store_location, const string& store_address, const string& depot_id, const double& latitude, const double& longitude);
        void displayStores();
    

    private:
        vector<unique_ptr<Store>> stores;

};

#endif