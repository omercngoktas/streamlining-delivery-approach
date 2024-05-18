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

    // creating ant colony and generating routes randomly
    AntColony antColony = AntColony(numOfAnts, vehicleCapacity);
    antColony.generateRoutes(storesManager, shipmentManager, vehicleManager, distanceDurationManager);
    antColony.displayAnts();

    // creating heuristic matrix based on distance between stores
    HeuristicMatrix heuristicMatrix(storesManager, distanceDurationManager, shipmentManager);
    heuristicMatrix.displayHeuristicMatrix();
    
    // creating pheromone matrix based on shipments
    PheromoneMatrix pheromoneMatrix(shipmentManager.getShipments(), storesManager.getStores());
    for(const auto& ant : antColony.getAnts()) {
        pheromoneMatrix.buildPheromoneMatrix(*ant);
    }
    pheromoneMatrix.showPheromoneMatrix();

    int numberOfIterations = 2000;
    int numberOfAnts = 50;
    Ant *bestAnt = new Ant();

    // update pheromone matrix based on the best ants of each iteration
    for(int i = 0; i < numberOfIterations; i++) {
        AntColony antColonyPheromone = AntColony(numberOfAnts, vehicleCapacity);
        antColonyPheromone.generateRoutesBasedOnPheromoneMatrix(storesManager, shipmentManager, pheromoneMatrix, distanceDurationManager, heuristicMatrix);

        // update pheromone matrix based on the best ants of each iteration
        // pheromoneMatrix.updatePheromoneMatrix(antColonyPheromone.getBestAnt());
        pheromoneMatrix.evaporatePheromoneMatrix(antColonyPheromone);
        pheromoneMatrix.pheromoneDeposition(antColonyPheromone);

        // if bestAnt is NULL or the fitness value of the best ant of the current iteration is better than the best ant of the previous iteration
        // but checking bestAnt == NULL does not work
        if(bestAnt->getFitnessValue() == 0 || antColonyPheromone.getBestAnt().getFitnessValue() < bestAnt->getFitnessValue()) {
            cout << "------------------------------------" << endl;
            cout << "Fitness value of the best ant of the current iteration: " << antColonyPheromone.getBestAnt().getFitnessValue() << " Iteration: " << i << endl;
            cout << "Current bestAnt fitness value: " << bestAnt->getFitnessValue() << endl;
            // best ant from antColonyPheromone will be deleted after the scope of the for loop ends
            // so we need to copy all the values of the best ant to the bestAnt
            bestAnt->setFitnessValue(antColonyPheromone.getBestAnt().getFitnessValue());
            bestAnt->clearRoutes();
            for(const auto& route : antColonyPheromone.getBestAnt().getRoutes()) {
                bestAnt->insertRoute(*route);
            }

            cout << "----------------------" << endl;
            
        }
        if(i %10 == 0) {
            cout << "Iteration: " << i << endl;
            cout << "Current best ant fitness value: " << antColonyPheromone.getBestAnt().getFitnessValue() << endl;
        }
    }

    bestAnt->displayRoutes();

    pheromoneMatrix.showPheromoneMatrix();

    return 0;
}
