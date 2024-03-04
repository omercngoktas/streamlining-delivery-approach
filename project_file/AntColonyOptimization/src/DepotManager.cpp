#include <iostream>
#include "../include/DepotManager.h"
using namespace std;

Depot::Depot(string depot_id, string depot_location, string depot_address, double latitude, double longitude)
    : depot_id(depot_id), depot_location(depot_location), depot_address(depot_address), latitude(latitude), longitude(longitude) {
}

// Setters
void Depot::setDepotId(const string& depot_id) { this->depot_id = depot_id; }
void Depot::setDepotLocation(const string& depot_location) { this->depot_location = depot_location; }
void Depot::setDepotAddress(const string& depot_address) { this->depot_address = depot_address; }
void Depot::setLatitude(const double& latitude) { this->latitude = latitude; }
void Depot::setLongitude(const double& longitude) { this->longitude = longitude; }
// Getters
string Depot::getDepotId() const { return depot_id; }
string Depot::getDepotLocation() const { return depot_location; }
string Depot::getDepotAddress() const { return depot_address; }
double Depot::getLatitude() const { return latitude; }
double Depot::getLongitude() const { return longitude; }

void DepotManager::setDepot(const string& depot_id, const string& depot_location, const string& depot_address, const double& latitude, const double& longitude) {
    depot = std::make_unique<Depot>(depot_id, depot_location, depot_address, latitude, longitude);
}

void DepotManager::displayDepot() {
    cout << "Depot ID: " << depot->getDepotId() << "\t";
    cout << "Depot Location: " << depot->getDepotLocation() << "\t";
    cout << "Depot Address: " << depot->getDepotAddress() << "\t";
    cout << "Latitude: " << depot->getLatitude() << "\t";
    cout << "Longitude: " << depot->getLongitude() << endl;
}