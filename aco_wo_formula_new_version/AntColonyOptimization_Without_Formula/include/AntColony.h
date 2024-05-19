#ifndef ANT_COLONY
#define ANT_COLONY

#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <iomanip> // Include for std::setw and std::left
#include <math.h>  // for pow()

#include "StoreManager.h"
#include "VehicleManager.h"
#include "DistanceDurationManager.h"
#include "ShipmentManager.h"
#include "RouteManager.h"
#include "DepotManager.h"

using namespace std;

class PheromoneMatrix;
class Ant;
class AntColony;
class HeuristicMatrix;

class Ant {
    public:
        Ant() = default;
        Ant(int vehicleCapacity) : vehicleCapacity(vehicleCapacity) {}
        ~Ant() = default;
        void generateRoute(const StoreManager& storeManager, const ShipmentManager& shipmentManager, const VehicleManager& vehicleManager, const DistanceDurationManager& distanceDurationManager);
        void addRoute(const Route& route) { routes.push_back(make_unique<Route>(route)); }
        void insertRoute(const Route& route) { routes.push_back(make_unique<Route>(route)); }
        void setRoutes(vector<unique_ptr<Route>> routes) { this->routes = move(routes); }
        void clearRoutes() { routes.clear(); }
        // get routes
        const vector<unique_ptr<Route>>& getRoutes() const { return routes; }
        // Getters
        int getNumberOfVehicleUsed() const { return numberOfVehicleUsed; }
        double getTotalDistance() const { return totalDistance; }
        double getTotalDuration() const { return totalDuration; }
        int getTotalPaletteCount() const { return totalPaletteCount; }
        int getFitnessValue() const { return fitnessValue; }
        // Setters
        void setNumberOfVehicleUsed(int numberOfVehicleUsed) { this->numberOfVehicleUsed = numberOfVehicleUsed; }
        void setTotalDistance(double totalDistance) { this->totalDistance = totalDistance; }
        void setTotalDuration(double totalDuration) { this->totalDuration = totalDuration; }
        void setTotalPaletteCount(int totalPaletteCount) { this->totalPaletteCount = totalPaletteCount; }
        void setFitnessValue(int fitnessValue) { this->fitnessValue = fitnessValue; }
        void calculateFitnessValue();
        void calculateTotalRouteDistance(const DistanceDurationManager& distanceDurationManager);
        // pheromone functions
        double getPheromone() const { return pheromone; }
        void setPheromone(double pheromone) { this->pheromone = pheromone; }
        void displayRoutes() const;
        // generate route based on pheromone matrix
        void generateRouteBasedOnPheromoneMatrix(const StoreManager& storeManager, const ShipmentManager& shipmentManager, PheromoneMatrix& pheromoneMatrix, const HeuristicMatrix& heuristicMatrix);
        void generateBestRoute(StoreManager& storeManager, ShipmentManager& shipmentManager, DepotManager& depotManager, DistanceDurationManager& distanceDurationManager);
        void removeStoreFromShipment(string storeId);

    private:
        vector<unique_ptr<Route>> routes;
        int vehicleCapacity;
        int numberOfVehicleUsed;
        double totalDistance;
        double totalDuration;
        int totalPaletteCount;
        int fitnessValue;
        double pheromone;
};

class AntColony {
    public:
        AntColony() = default;
        AntColony(int numOfAnts, int vehicleCapacity);
        ~AntColony() = default;
        void generateRoutes(const StoreManager& storeManager, const ShipmentManager& shipmentManager, const VehicleManager& vehicleManager, const DistanceDurationManager& distanceDurationManager);
        void displayRoutes() const;
        void displayAnts() const;
        void sortAntsByFitnessValue();
        Ant& getAnt(int index) { return *ants[index]; }
        // return ants
        const vector<unique_ptr<Ant>>& getAnts() const { return ants; }
        // generating routes based on pheromone matrix
        void generateRoutesBasedOnPheromoneMatrix(const StoreManager& storeManager, const ShipmentManager& shipmentManager, PheromoneMatrix& pheromoneMatrix, const DistanceDurationManager& distanceDurationManager, const HeuristicMatrix& heuristicMatrix);
        void insertAnt(Ant& ant);
        Ant& getBestAnt() { return *ants[0]; }

    private:
        vector<unique_ptr<Ant>> ants;
        int numOfAnts;
        int vehicleCapacity;
};

class PheromoneMatrix {
    public:
        PheromoneMatrix() = default;
        // PheromoneMatrix(Store)
        ~PheromoneMatrix() = default;
        // constructor with unique_ptr Store vector
        PheromoneMatrix(const vector<unique_ptr<Shipment>>& shipments, const vector<unique_ptr<Store>>& stores);
        void showPheromoneMatrix() const;
        void buildPheromoneMatrix(Ant& ant);
        void evaporatePheromoneMatrix(const AntColony& antColonyPheromone);
        
        string getNextStoreByPheromone(string storeId, vector<Shipment>& remainedShipments);
        void pheromoneDeposition(AntColony& antColonyPheromone);


    private:
        vector<vector<double>> pheromoneMatrix;
        vector<Store> stores;
        vector<Shipment> shipments;
};

class HeuristicMatrix {
    public:
        HeuristicMatrix() = default;
        ~HeuristicMatrix() = default;
        HeuristicMatrix(const StoreManager& storeManager, DistanceDurationManager& distanceDurationManager, const ShipmentManager& shipmentManager);
        void displayHeuristicMatrix() const;
        double getHeuristicValue(string storeId1, string storeId2) const;
        void displayStores() const;
        vector<vector<double>> getHeuristicMatrix() const { return heuristicMatrix; }

    private:
        vector<vector<double>> heuristicMatrix;
        vector<Store> stores;
};

#endif