#ifndef VEHICLE_MANAGER
#define VEHICLE_MANAGER

using namespace std;

class VehicleManager {
    public:
        VehicleManager();
        ~VehicleManager();
};

class Vehicle {
    public:
        string vehicle_id;
        string depot_id;
        string status;
        int pallette_capacity;
        Vehicle();
        ~Vehicle();
};


#endif