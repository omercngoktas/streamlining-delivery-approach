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

void writeToFile(const string& fileName, Ant& ant, Depot& depot) {
    // routes will be written to csv file
    // column names: route, latitude, longitude
    ofstream file(fileName);
    
    // route number will be determined by for loop from 1 to number of routes
    int routeNumber = 1;
    file << "route,latitude,longitude" << endl;

    for(const auto& route : ant.getRoutes()) {
        file << routeNumber << "," << depot.getLatitude() << "," << depot.getLongitude() << endl;
        for(const auto& store : route->getStores()) {
            file << routeNumber << "," <<  store->getLatitude() << "," << store->getLongitude() << endl;
        }
        file << routeNumber << "," << depot.getLatitude() << "," << depot.getLongitude() << endl;
        routeNumber++;
    }
}

int main(int argc, char* argv[]) {
    for(int iterationNumber = 0; iterationNumber < atoi(argv[8]); iterationNumber++) {

    if(argc != 9) {
        cout << "Usage: " << argv[0] << " <year> <month> <day> <numOfInitialAnts> <vehicleCapacity> <numOfIterations> <numOfAnts>" << endl;
        return 1;
    }

    cout << "argc: " << argc << endl;

    srand(time(0));

    // convert to int
    int year = atoi(argv[1]);
    int month = atoi(argv[2]);
    int day = atoi(argv[3]);

    int numOfAnts = atoi(argv[4]);
    int vehicleCapacity = atoi(argv[5]);

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

    // // creating heuristic matrix based on distance between stores
    HeuristicMatrix heuristicMatrix(storesManager, distanceDurationManager, shipmentManager);
    // heuristicMatrix.displayHeuristicMatrix();
    
    // creating pheromone matrix based on shipments
    PheromoneMatrix pheromoneMatrix(shipmentManager.getShipments(), storesManager.getStores());
    for(const auto& ant : antColony.getAnts()) {
        pheromoneMatrix.buildPheromoneMatrix(*ant);
    }
    // pheromoneMatrix.showPheromoneMatrix();

    int numberOfIterations = atoi(argv[6]);
    int numberOfAnts = atoi(argv[7]);
    
    Ant *bestAnt = new Ant();

    cout << "Year: " << year << " Month: " << month << " Day: " << day << endl;
    cout << "Number of initial ants: " << numOfAnts << endl;
    cout << "Vehicle capacity: " << vehicleCapacity << endl;
    cout << "Number of iterations: " << numberOfIterations << endl;
    cout << "Number of ants: " << numberOfAnts << endl;

    // antColony.getAnts()[0]->displayRoutes();

    cout << "Size of pheromone matrix: " << pheromoneMatrix.getPheromoneMatrix().size() << endl;
    cout << "Size of heuristic matrix: " << heuristicMatrix.getHeuristicMatrix().size() << " x " << heuristicMatrix.getHeuristicMatrix()[0].size() << endl;

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
    cout << "Best ant fitness value: " << bestAnt->getFitnessValue() << endl;
    cout << "Best ant total distance: " << bestAnt->getTotalDistance() << endl;

    // current clock time hour and minute to variable
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentHour = ltm->tm_hour;
    int currentMinute = ltm->tm_min;
    

    //file name is iteration_number_numOfInitialAnts_numOfIterations_numOfAnts_routes_wo_formula.csv
    string fileName = to_string(iterationNumber) + "_" + to_string(numOfAnts) + "_" + to_string(numberOfIterations) + "_" + to_string(numberOfAnts) + "_routes_with_formula.csv";
    
    writeToFile("/Users/omercangoktas/Desktop/github/routing-and-distribution-optimization/aco_wo_formula_new_version/output/test_results/" + fileName , *bestAnt, depotManager.getDepot());
    }
    return 0;
}

// make run_with ARGS="2024 3 4 200 18 100 5"