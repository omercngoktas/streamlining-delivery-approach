#ifndef STORE
#define STORE

#include <string>
#include <iostream>

using namespace std;

class Store {
    public:
        string store_id, store_location, store_address, depot_id;
        double latitude, longitude;
        Store();
        Store(const string& store_id, const string& store_location, const string& store_address, const double& latitude, const double& longitude, const string& depot_id);
        
        ~Store();
};

#endif