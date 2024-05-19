#ifndef VEHICLE_MANAGER
#define VEHICLE_MANAGER

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cctype>

using namespace std;

class Vehicle {
    public:
        Vehicle() = default;
        Vehicle(const string& vehicle_id, const string& depot_id, const string& status, const int& palette_capacity);
        ~Vehicle() = default;
        // Getters
        string getVehicleId() const { return this->vehicle_id; }
        string getDepotId() const { return this->depot_id; }
        string getStatus() const { return this->status; }
        int getPaletteCapacity() const { return this->palette_capacity; }
        // Setters
        void setVehicleId(string vehicle_id) { this->vehicle_id = vehicle_id; }
        void setDepotId(string depot_id) { this->depot_id = depot_id; }
        void setStatus(string status) { this->status = status; }
        void setPaletteCapacity(int palette_capacity) { this->palette_capacity = palette_capacity; }
    private:
        string vehicle_id;
        string depot_id;
        string status;
        int palette_capacity;
};

class VehicleManager {
    public:
        VehicleManager() = default;
        ~VehicleManager() = default;
        void addVehicle(const string& vehicle_id, const string& depot_id, const string& status, const int& palette_capacity);
        void displayVehicles();
        vector<unique_ptr<Vehicle>>& getVehicles() { return this->vehicles; }
    private:
        vector<unique_ptr<Vehicle>> vehicles;
};

#endif