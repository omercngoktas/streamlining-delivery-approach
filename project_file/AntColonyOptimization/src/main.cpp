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
    AntColony antColony = AntColony(numOfAnts, vehicleCapacity);

    dbConnector.fetchStores(storesManager); // Fetch stores from the database and store them in storesManager vector
    dbConnector.fetchDepot(depotManager); // Fetch depot from the database and store them in depotManager vector
    dbConnector.fetchVehicles(vehicleManager); // Fetch vehicles from the database and store them in vehicleManager vector
    dbConnector.fetchStoresDistancesDurations(distanceDurationManager, year, month, day);
    dbConnector.fetchDepotDistancesDurations(distanceDurationManager, year, month, day);
    dbConnector.fetchShipments(shipmentManager, year, month, day); // Fetch shipments from the database and store them in shipmentManager vector

    antColony.generateRoutes(storesManager, shipmentManager, vehicleManager, distanceDurationManager);
    
    // antColony.displayRoutes();

    cout << "Total palette count: " << shipmentManager.getTotalPaletteCount() << endl;


    antColony.displayAnts();


    PheromoneMatrix pheromoneMatrix(shipmentManager.getShipments(), storesManager.getStores());

    for(const auto& ant : antColony.getAnts()) {
        // convert auto to Ant
        pheromoneMatrix.updatePheromoneMatrix(*ant);
    }
    
    cout << "--------------------------------------------------------------------------------------------------------\n";
    pheromoneMatrix.showPheromoneMatrix();
    cout << "--------------------------------------------------------------------------------------------------------\n";

    int numberOfIterations = 50;
    int numberOfAnts = 20;

    AntColony bestAntColony = AntColony(numberOfAnts, vehicleCapacity);

    // update pheromone matrix based on the best ants of each iteration
    for(int i = 0; i < numberOfIterations; i++) {
        AntColony antColonyPheromone = AntColony(numberOfAnts, vehicleCapacity);
        antColonyPheromone.generateRoutesBasedOnPheromoneMatrix(storesManager, shipmentManager, pheromoneMatrix, distanceDurationManager);
        antColonyPheromone.sortAntsByFitnessValue();
        antColonyPheromone.displayAnts();
    }

    bestAntColony.displayAnts();




    return 0;
}
