#ifndef ANT_COLONY
#define ANT_COLONY

#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <iomanip> // Include for std::setw and std::left


#include "StoreManager.h"
#include "VehicleManager.h"
#include "DistanceDurationManager.h"
#include "ShipmentManager.h"
#include "RouteManager.h"

using namespace std;

class Ant {
    public:
        Ant() = default;
        Ant(int vehicleCapacity) : vehicleCapacity(vehicleCapacity) {}
        ~Ant() = default;
        void generateRoute(const StoreManager& storeManager, const ShipmentManager& shipmentManager, const VehicleManager& vehicleManager, const DistanceDurationManager& distanceDurationManager);
        void addRoute(const Route& route) { routes.push_back(make_unique<Route>(route)); }
        // get routes
        const vector<unique_ptr<Route>>& getRoutes() const { return routes; }
        // Getters
        int getNumberOfVehicleUsed() const { return numberOfVehicleUsed; }
        double getTotalDistance() const { return totalDistance; }
        double getTotalDuration() const { return totalDuration; }
        int getTotalPaletteCount() const { return totalPaletteCount; }
        // Setters
        void setNumberOfVehicleUsed(int numberOfVehicleUsed) { this->numberOfVehicleUsed = numberOfVehicleUsed; }
        void setTotalDistance(double totalDistance) { this->totalDistance = totalDistance; }
        void setTotalDuration(double totalDuration) { this->totalDuration = totalDuration; }
        void setTotalPaletteCount(int totalPaletteCount) { this->totalPaletteCount = totalPaletteCount; }
    private:
        vector<unique_ptr<Route>> routes;
        int vehicleCapacity;
        int numberOfVehicleUsed;
        double totalDistance;
        double totalDuration;
        int totalPaletteCount;
};

class AntColony {
    public:
        AntColony() = default;
        AntColony(int numOfAnts, int vehicleCapacity);
        ~AntColony() = default;
        void generateRoutes(const StoreManager& storeManager, const ShipmentManager& shipmentManager, const VehicleManager& vehicleManager, const DistanceDurationManager& distanceDurationManager);
        void displayRoutes() const;
        void displayAnts() const;
    private:
        vector<unique_ptr<Ant>> ants;
        int numOfAnts;
        int vehicleCapacity;
        
};

#endif