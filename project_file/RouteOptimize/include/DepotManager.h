#ifndef DEPOT_MANAGER
#define DEPOT_MANAGER

#include <iostream>
#include <vector>

using namespace std;

class DepotManager {
    public:
        DepotManager();
        ~DepotManager();
        vector<Depot> getDepots();
};

class Depot {
    public:
        string depot_id, depot_location, depot_address;
        double latitude, longitude;
        Depot();
        ~Depot();
};

#endif