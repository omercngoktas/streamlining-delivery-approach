#include <iostream>
#include "../include/VehicleManager.h"
using namespace std;

Vehicle::Vehicle(const string& vehicle_id, const string& depot_id, const string& status, const int& palette_capacity)
    : vehicle_id(vehicle_id), depot_id(depot_id), status(status), palette_capacity(palette_capacity){

}

// Setters
void Vehicle::setVehicleId(const string& vehicleId) { this->vehicle_id = vehicle_id; }
void Vehicle::setDepotId(const string& depot_id) { this->vehicle_id = vehicle_id; }
void Vehicle::setStatus(const string& status) { this->vehicle_id = vehicle_id; }
void Vehicle::setPaletteCapacity(const int& palette_capacity) { this->palette_capacity = palette_capacity; }
// Getters
string Vehicle::getVehicleId() const { return this->vehicle_id; }
string Vehicle::getDepotId() const { return this->depot_id; }
string Vehicle::getStatus() const { return this->status; }
int Vehicle::getPaletteCapacity() const { return this->palette_capacity; }

void VehicleManager::addVehicle(const string& vehicle_id, const string& depot_id, const string& status, const int& palette_capacity) {
    auto vehicle = make_unique<Vehicle>(vehicle_id, depot_id, status, palette_capacity);
    vehicles.push_back(move(vehicle));
}

void VehicleManager::displayVehicles() {
    for (const auto& vehicle : vehicles) {
        cout << "Vehicle ID: " << vehicle->getVehicleId() << "\t";
        cout << "Depot ID: " << vehicle->getDepotId() << "\t";
        cout << "Status: " << vehicle->getStatus() << "\t";
        cout << "Palette Capacity: " << vehicle->getPaletteCapacity() << endl;

    }
}