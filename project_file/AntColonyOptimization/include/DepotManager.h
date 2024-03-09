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
        
        // Getters
        string getDepotId() const { return this->depot_id; }
        string getDepotLocation() const { return this->depot_location; }
        string getDepotAddress() const { return this->depot_address; }
        double getLatitude() const { return this->latitude; }
        double getLongitude() const { return this->longitude; }
        // Setters
        void setDepotId(string depot_id) { this->depot_id = depot_id; }
        void setDepotLocation(string depot_location) { this->depot_location = depot_location; }
        void setDepotAddress(string depot_address) { this->depot_address = depot_address; }
        void setLatitude(double latitude) { this->latitude = latitude; }
        void setLongitude(double longitude) { this->longitude = longitude; }
    
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