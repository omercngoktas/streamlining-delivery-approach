#include "../include/AntColony.h"

// Constructor for AntColony
AntColony::AntColony(int numOfAnts, int vehicleCapacity) {
    this->numOfAnts = numOfAnts;
    this->vehicleCapacity = vehicleCapacity;
}

// Generate routes for the ants
void AntColony::generateRoutes(const StoreManager& storeManager, const ShipmentManager& shipmentManager, const VehicleManager& vehicleManager, const DistanceDurationManager& distanceDurationManager) {
    
    for(int i = 0; i < numOfAnts; i++) {
        auto ant = std::make_unique<Ant>(vehicleCapacity);
        ant->generateRoute(storeManager, shipmentManager, vehicleManager, distanceDurationManager);
        ants.push_back(move(ant));
    }
}

// Generate a route for the ant
void Ant::generateRoute(const StoreManager& storeManager, const ShipmentManager& shipmentManager, const VehicleManager& vehicleManager, const DistanceDurationManager& distanceDurationManager) {
    cout << "Vehicle Capacity: " << vehicleCapacity << endl;

    vector<Store> unvisitedStores;
    for(const auto& store : storeManager.getStores()) { unvisitedStores.push_back(*store); }
    vector<Shipment> remainedShipments;
    for(const auto& shipment : shipmentManager.getShipments()) { if(shipment->getNoOfShipments() > 0) { remainedShipments.push_back(*shipment); } }

    int currentCapacity;

    while(!remainedShipments.empty()) {
        Shipment shipmentToBeAdded;
        
        currentCapacity = 0;
        cout << "--------------------------------------------------------" << endl;
        while(currentCapacity < this->vehicleCapacity && !remainedShipments.empty()) {
            cout << "Current vehicle capacity: " << currentCapacity << endl;
            // Select a random shipment
            Shipment shipment = shipmentManager.getRandShipment(remainedShipments);
            // when currentCapacity is enough and new shipment palette count makes it exceed the vehicle capacity
            if(currentCapacity < this->vehicleCapacity && currentCapacity + shipment.getNoOfShipments() >= this->vehicleCapacity) {
                cout << "Best shipment is needed!" << endl;
                // get best suitable shipment
                Shipment bestShipment = shipmentManager.getBestShipment(remainedShipments, this->vehicleCapacity - currentCapacity);
                // if bestShipment is not found
                if(bestShipment.getNoOfShipments() == 0) {
                    cout << "*** Best shipment not found! ***" << endl;
                    break;
                } else {
                    cout << "Best shipment found!" << endl;
                    // increase currentCapacity
                    currentCapacity += bestShipment.getNoOfShipments();
                    Route currentRoute;
                    currentRoute.addVisitPoint(storeManager.getStoreById(bestShipment.getStoreId()), bestShipment);
                    routes.push_back(make_unique<Route>(currentRoute));
                    // remove the shipment from remainedShipments
                    shipmentManager.removeShipment(remainedShipments, bestShipment.getShipmentId());
                }
            } else {
                // increase currentCapacity
                currentCapacity += shipment.getNoOfShipments();
                Route currentRoute;
                currentRoute.addVisitPoint(storeManager.getStoreById(shipment.getStoreId()), shipment);
                routes.push_back(make_unique<Route>(currentRoute));
                // remove the shipment from remainedShipments
                shipmentManager.removeShipment(remainedShipments, shipment.getShipmentId());
            }
        }
        
        cout << "--- Route is completed ---" << endl;
    }
}