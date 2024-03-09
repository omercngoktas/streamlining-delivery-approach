#ifndef ANT_COLONY
#define ANT_COLONY

#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()

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
        
    private:
        vector<unique_ptr<Route>> routes;
        int vehicleCapacity;
};

class AntColony {
    public:
        AntColony() = default;
        AntColony(int numOfAnts, int vehicleCapacity);
        ~AntColony() = default;
        void generateRoutes(const StoreManager& storeManager, const ShipmentManager& shipmentManager, const VehicleManager& vehicleManager, const DistanceDurationManager& distanceDurationManager);


    private:
        vector<unique_ptr<Ant>> ants;
        int numOfAnts;
        int vehicleCapacity;
        
};

#endif