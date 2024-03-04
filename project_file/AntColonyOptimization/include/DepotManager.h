#ifndef DEPOT_MANAGER
#define DEPOT_MANAGER

#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Depot {
    public:
        Depot(string depot_id, string depot_location, string depot_address, double latitude, double longitude);
        ~Depot() = default;
        // Setters
        void setDepotId(const string& depot_id);
        void setDepotLocation(const string& depot_location);
        void setDepotAddress(const string& depot_address);
        void setLatitude(const double& latitude);
        void setLongitude(const double& longitude);
        // Getters
        string getDepotId() const;
        string getDepotLocation() const;
        string getDepotAddress() const;
        double getLatitude() const;
        double getLongitude() const;
    
    private:
        string depot_id;
        string depot_location;
        string depot_address;
        double latitude;
        double longitude;
};

class DepotManager {
    public:
        DepotManager() = default;
        ~DepotManager() = default;
        void setDepot(const string& depot_id, const string& depot_location, const string& depot_address, const double& latitude, const double& longitude);
        void displayDepot();

    private:
        unique_ptr<Depot> depot;

};



#endif