#include <iostream>
#include <vector>

#include "../include/DatabaseManager.h"
#include "../include/StoreManager.h"
#include "../include/DepotManager.h"
#include "../include/ShipmentManager.h"
#include "../include/VehicleManager.h"
#include "../include/DistanceDurationManager.h"
#include "../include/AntColony.h"

using namespace std;

int main() {
    srand(time(0));

    int year = 2024;
    int month = 3;
    int day = 4;

    int numOfAnts = 50;
    int vehicleCapacity = 18;

    const char* host = "localhost";
    const char* user = "omercngoktas";
    const char* password = "mwanamboka";
    const char* database = "mydatabase";
    // Create the MySQLConnector object with connection details
    MySQLConnector dbConnector(host, user, password, database);
    
    StoreManager storesManager;
    DepotManager depotManager;
    ShipmentManager shipmentManager;
    VehicleManager vehicleManager;
    DistanceDurationManager distanceDurationManager;
    
    dbConnector.fetchStores(storesManager); // Fetch stores from the database and store them in storesManager vector
    dbConnector.fetchDepot(depotManager); // Fetch depot from the database and store them in depotManager vector
    dbConnector.fetchVehicles(vehicleManager); // Fetch vehicles from the database and store them in vehicleManager vector
    dbConnector.fetchStoresDistancesDurations(distanceDurationManager, year, month, day);
    dbConnector.fetchDepotDistancesDurations(distanceDurationManager, year, month, day);
    dbConnector.fetchShipments(shipmentManager, year, month, day); // Fetch shipments from the database and store them in shipmentManager vector

    // creating ant colony and generating routes
    AntColony antColony = AntColony(numOfAnts, vehicleCapacity);
    antColony.generateRoutes(storesManager, shipmentManager, vehicleManager, distanceDurationManager);
    antColony.displayAnts();

    // creating heuristic matrix based on distance between stores
    HeuristicMatrix heuristicMatrix(storesManager, distanceDurationManager, shipmentManager);
    heuristicMatrix.displayHeuristicMatrix();
    
    // creating pheromone matrix based on shipments
    PheromoneMatrix pheromoneMatrix(shipmentManager.getShipments(), storesManager.getStores());
    for(const auto& ant : antColony.getAnts()) {
        pheromoneMatrix.updatePheromoneMatrix(*ant);
    }
    pheromoneMatrix.showPheromoneMatrix();

    int numberOfIterations = 10;
    int numberOfAnts = 20;
    Ant bestAnt;

    // update pheromone matrix based on the best ants of each iteration
    for(int i = 0; i < numberOfIterations; i++) {
        AntColony antColonyPheromone = AntColony(numberOfAnts, vehicleCapacity);
        antColonyPheromone.generateRoutesBasedOnPheromoneMatrix(storesManager, shipmentManager, pheromoneMatrix, distanceDurationManager);
        antColonyPheromone.sortAntsByFitnessValue();

        // update pheromone matrix based on the best ants of each iteration
        pheromoneMatrix.updatePheromoneMatrix(antColonyPheromone.getBestAnt());

        if(i == numberOfIterations - 1) {
            antColonyPheromone.displayAnts();
        }
        // if(i % 50 == 0) {
        //     antColonyPheromone.displayAnts();
        // }
    }

    pheromoneMatrix.showPheromoneMatrix();

    



    return 0;
}
