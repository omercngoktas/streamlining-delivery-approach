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

using namespace std;

class Store {
    public:
        string store_id;
        string store_location;
        string store_address;
        string depot_id;
        double latitude;
        double longitude;
        Store();
        Store(const string& store_id, const string& store_location, const string& store_address, const double& latitude, const double& longitude, const string& depot_id);
        ~Store();
};

class StoreManager {
    public:
        StoreManager();
        ~StoreManager();
        void readStores(const string& filename);
        void printStores() const;

    private:
        vector<Store> stores;

};

#endif