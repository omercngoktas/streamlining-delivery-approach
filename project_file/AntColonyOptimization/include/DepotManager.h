#ifndef DEPOT_MANAGER
#define DEPOT_MANAGER

#include <iostream>
#include <vector>

using namespace std;

class DepotManager {
    public:
        DepotManager();
        ~DepotManager();
};

class Depot {
    public:
        string depot_id;
        string depot_location;
        string depot_address;
        double latitude;
        double longitude;
        Depot();
        ~Depot();
};

#endif