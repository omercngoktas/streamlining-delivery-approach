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
        
        // Setters
        void setVehicleId(const string& vehicleId);
        void setDepotId(const string& depotId);
        void setStatus(const string& status);
        void setPaletteCapacity(const int& paletteCapacity);
        // Getters
        string getVehicleId() const;
        string getDepotId() const;
        string getStatus() const;
        int getPaletteCapacity() const;
        
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

    private:
        vector<unique_ptr<Vehicle>> vehicles;

};

#endif