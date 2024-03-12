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

    // calculate the total distance of the routes and other things
    for(const auto& ant : ants) {
        for(const auto& route : ant->getRoutes()) {
            route->calculateTotalDistance(distanceDurationManager);
            route->calculateTotalDuration(distanceDurationManager);
            route->calculateTotalPaletteCount();
            
        }
    }

    // calculate the total distance of the routes and other things for the ants
    for(const auto& ant : ants) {
        for(const auto& route : ant->getRoutes()) {
            ant->setTotalDistance(ant->getTotalDistance() + route->getTotalDistance());
            ant->setTotalDuration(ant->getTotalDuration() + route->getTotalDuration());
            ant->setTotalPaletteCount(ant->getTotalPaletteCount() + route->getTotalPaletteCount());
        }
    }

}

// Generate a route for the ant
void Ant::generateRoute(const StoreManager& storeManager, const ShipmentManager& shipmentManager, const VehicleManager& vehicleManager, const DistanceDurationManager& distanceDurationManager) {
    // cout << "Vehicle Capacity: " << vehicleCapacity << endl;
    vector<Store> unvisitedStores;
    for(const auto& store : storeManager.getStores()) { unvisitedStores.push_back(*store); }
    vector<Shipment> remainedShipments;
    for(const auto& shipment : shipmentManager.getShipments()) { if(shipment->getNoOfShipments() > 0) { remainedShipments.push_back(*shipment); } }
    int currentCapacity;

    while(!remainedShipments.empty()) {
        Shipment shipmentToBeAdded;
        Route route;
        currentCapacity = 0;
        // cout << "--------------------------------------------------------" << endl;
        while(currentCapacity < this->vehicleCapacity && !remainedShipments.empty()) {
            // cout << "Current vehicle capacity: " << currentCapacity << endl;
            // Select a random shipment
            Shipment shipment = shipmentManager.getRandShipment(remainedShipments);
            // when currentCapacity is enough and new shipment palette count makes it exceed the vehicle capacity
            if(currentCapacity < this->vehicleCapacity && currentCapacity + shipment.getNoOfShipments() >= this->vehicleCapacity) {
                // cout << "Best shipment is needed!" << endl;
                // get best suitable shipment
                Shipment bestShipment = shipmentManager.getBestShipment(remainedShipments, this->vehicleCapacity - currentCapacity);
                // if bestShipment is not found
                if(bestShipment.getNoOfShipments() == 0) {
                    // cout << "*** Best shipment not found! ***" << endl;
                    break;
                } else {
                    // cout << "Best shipment found!" << endl;
                    // increase currentCapacity
                    currentCapacity += bestShipment.getNoOfShipments();
                    shared_ptr<Shipment> bestShipmentPtr = make_shared<Shipment>(bestShipment);
                    Store store = storeManager.getStoreById(bestShipment.getStoreId());
                    shared_ptr<Store> storePtr = make_shared<Store>(store);
                    
                    route.addVisitPoint(storePtr, bestShipmentPtr);
                    // remove the shipment from remainedShipments
                    shipmentManager.removeShipment(remainedShipments, bestShipment.getShipmentId());
                }
            } else {
                // increase currentCapacity
                currentCapacity += shipment.getNoOfShipments();
                shared_ptr<Shipment> bestShipmentPtr = make_shared<Shipment>(shipment);
                Store store = storeManager.getStoreById(shipment.getStoreId());
                shared_ptr<Store> storePtr = make_shared<Store>(store);
                
                route.addVisitPoint(storePtr, bestShipmentPtr);
                // remove the shipment from remainedShipments
                shipmentManager.removeShipment(remainedShipments, shipment.getShipmentId());
            }
        }
        // add route to the ant
        this->addRoute(route);
        // add used vehicle
        this->numberOfVehicleUsed++;
        // cout << "--- Route is completed ---" << endl;
    }
}

// Display the routes
void AntColony::displayRoutes() const {
    for(const auto& ant : ants) {
        cout << "Ant" << endl;
        for(const auto& route : ant->getRoutes()) {
            route->displayRoute();
        }
    }
}

// Display the ants using iomanip
void AntColony::displayAnts() const {
    using std::cout;
    using std::endl;
    using std::left;
    using std::setw;

    // Calculate the total width based on the widths used in setw plus separators
    int totalWidth = 10 + 16 + 16 + 16 + 19 + 11; // Sum of all setw widths + separators('|' and spaces)
    
    // Function to print dashes
    auto printLine = [&]() {
        cout << "+"; // Start border
        for (int i = 1; i < totalWidth; i++) { // One less than totalWidth to account for starting '+'
            cout << "-";
        }
        cout << "+" << endl; // End border
    };

    printLine();
    cout << "| " << setw(10) << left << "Ant ID"
         << "| " << setw(16) << "Vehicle Used"
         << "| " << setw(16) << "Total Distance"
         << "| " << setw(16) << "Total Duration"
         << "| " << setw(19) << "Total Palette Count |" << endl;
    printLine();
    for (int i = 0; i < this->ants.size(); i++) {
        cout << "| " << setw(10) << left << i
             << "| " << setw(16) << this->ants[i]->getNumberOfVehicleUsed()
             << "| " << setw(16) << this->ants[i]->getTotalDistance()
             << "| " << setw(16) << this->ants[i]->getTotalDuration()
             << "| " << setw(19) << this->ants[i]->getTotalPaletteCount() << " |" << endl;
    }
    printLine();
}