#include <iostream>
#include "../include/DatabaseManager.h"
#include "../include/StoreManager.h"
// #include "../include/DepotManager.h"
// #include "../include/ShipmentManager.h"
// #include "../include/RouteManager.h"
// #include "../include/GeneticAlgorithm.h"
// #include "../include/VehicleManager.h"
using namespace std;

int main(int argc, char* argv[]) {
    StoreManager store_manager;
    store_manager.readStores("./bin/data/store/stores.csv");
    store_manager.printStores();
    return 0;
}
