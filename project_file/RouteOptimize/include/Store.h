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
        ~Store();
};

#endif