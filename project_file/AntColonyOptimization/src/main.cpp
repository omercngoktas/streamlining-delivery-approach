#include <iostream>
#include "../include/DatabaseManager.h"
#include "../include/StoreManager.h"
#include "../include/DepotManager.h"
#include "../include/ShipmentManager.h"
// #include "../include/RouteManager.h"
// #include "../include/GeneticAlgorithm.h"
#include "../include/VehicleManager.h"
#include <vector>
using namespace std;

int main() {
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


    dbConnector.fetchStores(storesManager);
    storesManager.displayStores();

    dbConnector.fetchDepot(depotManager);
    depotManager.displayDepot();

    dbConnector.fetchVehicles(vehicleManager);
    vehicleManager.displayVehicles();
    

    
    
    
    
    

    return 0;
}
