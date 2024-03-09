#include "../include/VehicleManager.h"

// Constructor for Vehicle
Vehicle::Vehicle(const string& vehicle_id, const string& depot_id, const string& status, const int& palette_capacity)
    : vehicle_id(vehicle_id), depot_id(depot_id), status(status), palette_capacity(palette_capacity){

}

// Add a vehicle to the vehicle manager
void VehicleManager::addVehicle(const string& vehicle_id, const string& depot_id, const string& status, const int& palette_capacity) {
    auto vehicle = make_unique<Vehicle>(vehicle_id, depot_id, status, palette_capacity);
    vehicles.push_back(move(vehicle));
}

// Display the vehicles
void VehicleManager::displayVehicles() {
    for (const auto& vehicle : vehicles) {
        cout << "Vehicle ID: " << vehicle->getVehicleId() << "\t";
        cout << "Depot ID: " << vehicle->getDepotId() << "\t";
        cout << "Status: " << vehicle->getStatus() << "\t";
        cout << "Palette Capacity: " << vehicle->getPaletteCapacity() << endl;
    }
}