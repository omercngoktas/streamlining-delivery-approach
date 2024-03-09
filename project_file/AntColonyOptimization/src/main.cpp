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
    AntColony antColony = AntColony(2, 18);


    dbConnector.fetchStores(storesManager); // Fetch stores from the database and store them in storesManager vector
    dbConnector.fetchDepot(depotManager); // Fetch depot from the database and store them in depotManager vector
    dbConnector.fetchVehicles(vehicleManager); // Fetch vehicles from the database and store them in vehicleManager vector
    
    dbConnector.fetchStoresDistancesDurations(distanceDurationManager, 2024, 3, 4);
    dbConnector.fetchDepotDistancesDurations(distanceDurationManager, 2024, 3, 4);

    

    dbConnector.fetchShipments(shipmentManager, 2024, 3, 4); // Fetch shipments from the database and store them in shipmentManager vector

    antColony.generateRoutes(storesManager, shipmentManager, vehicleManager, distanceDurationManager);
    
    
    
    

    return 0;
}
