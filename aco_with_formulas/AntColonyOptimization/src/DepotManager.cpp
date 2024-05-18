#include <iostream>
#include "../include/DepotManager.h"
using namespace std;

// Constructor for Depot
Depot::Depot(string depot_id, string depot_location, string depot_address, double latitude, double longitude)
    : depot_id(depot_id), depot_location(depot_location), depot_address(depot_address), latitude(latitude), longitude(longitude) {
}

// Set the depot
void DepotManager::setDepot(const string& depot_id, const string& depot_location, const string& depot_address, const double& latitude, const double& longitude) {
    depot = std::make_unique<Depot>(depot_id, depot_location, depot_address, latitude, longitude);
}

// Display the depot
void DepotManager::displayDepot() {
    cout << "Depot ID: " << depot->getDepotId() << "\t";
    cout << "Depot Location: " << depot->getDepotLocation() << "\t";
    cout << "Depot Address: " << depot->getDepotAddress() << "\t";
    cout << "Latitude: " << depot->getLatitude() << "\t";
    cout << "Longitude: " << depot->getLongitude() << endl;
}