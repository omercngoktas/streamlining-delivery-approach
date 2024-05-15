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

            // Assigning the pheromone value to the route
            int totalDistance = route->getTotalDistance();
            int totalStoresCount = route->getStores().size();
            // Convert one of the variables to double for floating-point division
            double pheromone = static_cast<double>(totalStoresCount) / totalDistance;
            route->setPheromone(pheromone);
        }
    }

    // calculate the total distance of the routes and other things for the ants
    for(const auto& ant : ants) {
        for(const auto& route : ant->getRoutes()) {
            ant->setTotalDistance(ant->getTotalDistance() + route->getTotalDistance());
            ant->setTotalDuration(ant->getTotalDuration() + route->getTotalDuration());
            ant->setTotalPaletteCount(ant->getTotalPaletteCount() + route->getTotalPaletteCount());
            ant->calculateFitnessValue();
            ant->setPheromone(static_cast<double>(shipmentManager.getShipments().size()) / ant.get()->getTotalDistance());
            
        }
    }
    // sort the ants by fitness value
    sortAntsByFitnessValue();
    
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
        currentCapacity = 0;
        Route route;
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
    int totalWidth = 10 + 16 + 16 + 16 + 19 + 17 + 9; // Sum of all setw widths + separators('|' and spaces)
    
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
         << "| " << setw(19) << "Total Palette Count"
         << "| " << setw(17) << "Fitness Value |" << endl;
    printLine();
    for (int i = 0; i < this->ants.size(); i++) {
        cout << "| " << setw(10) << left << i
             << "| " << setw(16) << this->ants[i]->getNumberOfVehicleUsed()
             << "| " << setw(16) << this->ants[i]->getTotalDistance()
             << "| " << setw(16) << this->ants[i]->getTotalDuration()
             << "| " << setw(19) << this->ants[i]->getTotalPaletteCount()
             << "| " << setw(14) << this->ants[i]->getFitnessValue() << "|" << endl;
    }
    printLine();
}

// Calculate the fitness value of the ant
void Ant::calculateFitnessValue() {
    int numberOfVehicleUsed = this->getNumberOfVehicleUsed();
    double totalDistance = this->getTotalDistance();
    double totalDuration = this->getTotalDuration();

    int fitness = (numberOfVehicleUsed * 500) + (totalDistance * 10) + (totalDuration * 3);
    this->setFitnessValue(fitness);
}

// Sort the ants by fitness value
void AntColony::sortAntsByFitnessValue() {
    std::sort(ants.begin(), ants.end(), [](const unique_ptr<Ant>& a, const unique_ptr<Ant>& b) {
        return a->getFitnessValue() < b->getFitnessValue();
    });
}

// Pheromone matrix constructor
PheromoneMatrix::PheromoneMatrix(const vector<unique_ptr<Shipment>>& shipments, const vector<unique_ptr<Store>>& stores) {
    // assign shipments and stores
    for(const auto& shipment : shipments) {
        this->shipments.push_back(*shipment);
    }
    for(const auto& store : stores) {
        this->stores.push_back(*store);
    }

    for(int i = 0; i < shipments.size(); i++) {
        vector<double> row;
        for(int j = 0; j < shipments.size(); j++) {
            row.push_back(0.01);
        }
        pheromoneMatrix.push_back(row);
    }
}

// Show the pheromone matrix
void PheromoneMatrix::showPheromoneMatrix() const {
    cout << "------------------------------------------------- PHEROMONE MATRIX -------------------------------------------------" << endl;

    for(const auto& shipment : shipments) {
        // print characters between index 3-6
        cout << shipment.getStoreId().substr(3, 6) << " ";
    }
    cout << endl;

    int i = 0;
    for(const auto& row : pheromoneMatrix) {
        for(const auto& element : row) {
            // print decimal point with 3 precision and use setw
            cout << fixed << setprecision(3) << setw(6) << element << " ";
        }
        cout << shipments[i++].getStoreId().substr(3, 6) << endl;
    }

    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
}

// display routes for ant
void Ant::displayRoutes() const {
    for(const auto& route : routes) {
        route->displayRoute();
    }
}

// update pheromone matrix
void PheromoneMatrix::updatePheromoneMatrix(Ant& ant) {
    double pheromone = ant.getPheromone();
    
    for(const auto& route : ant.getRoutes()) {
        for(int i = 0; i < route->getStores().size() - 1; i++) {
            int store1Index = -1;
            int store2Index = -1;
            for(int j = 0; j < stores.size(); j++) {
                if(stores[j].getStoreId() == route->getStores()[i]->getStoreId()) {
                    store1Index = j;
                }
                if(stores[j].getStoreId() == route->getStores()[i + 1]->getStoreId()) {
                    store2Index = j;
                }
            }
            //find index for store1 and store2 in the pheromone matrix
            for(int k = 0; k < shipments.size(); k++) {
                if(shipments[k].getStoreId() == route->getStores()[i]->getStoreId()) {
                    store1Index = k;
                }
                if(shipments[k].getStoreId() == route->getStores()[i + 1]->getStoreId()) {
                    store2Index = k;
                }
            }

            if(store1Index != -1 && store2Index != -1) {
                pheromoneMatrix[store1Index][store2Index] += pheromone;
                pheromoneMatrix[store2Index][store1Index] += pheromone;
            }

        }
    }
}

// update pheromone matrix for route
void PheromoneMatrix::updatePheromoneMatrixForRoute(string storeId1, string storeId2, double pheromone) {
    int store1Index = -1;
    int store2Index = -1;
    for(int i = 0; i < stores.size(); i++) {
        if(stores[i].getStoreId() == storeId1) {
            store1Index = i;
        }
        if(stores[i].getStoreId() == storeId2) {
            store2Index = i;
        }
    }
    if(store1Index != -1 && store2Index != -1) {
        pheromoneMatrix[store1Index][store2Index] = pheromone;
    }
}

// with the given storeId, returns the next storeId with the highest pheromone
string PheromoneMatrix::getNextStoreByPheromone(string storeId, vector<Shipment>& remainedShipments) {
    int storeIndex = -1;
    int highestPheromoneIndex = -1;
    double highestPheromone = 0.0;
    
    for(int i = 0; i < shipments.size(); i++) {
        if(shipments[i].getStoreId() == storeId) {
            storeIndex = i;
            break;
        }
    }

    for(int j = 0; j < pheromoneMatrix[storeIndex].size(); j++) {
        if(pheromoneMatrix[storeIndex][j] > highestPheromone) {
            for(Shipment shipment : remainedShipments) {
                if(shipment.getStoreId() == shipments[j].getStoreId()) {
                    highestPheromone = pheromoneMatrix[storeIndex][j];
                    highestPheromoneIndex = j;
                }
            }
        }
    }

    return shipments[highestPheromoneIndex].getStoreId();
}

// generate routes based on pheromone matrix for ant colony
void AntColony::generateRoutesBasedOnPheromoneMatrix(const StoreManager& storeManager, const ShipmentManager& shipmentManager, PheromoneMatrix& pheromoneMatrix, const DistanceDurationManager& distanceDurationManager) {
    for(int i = 0; i < numOfAnts; i++) {
        auto ant = std::make_unique<Ant>(vehicleCapacity);
        ant->generateRouteBasedOnPheromoneMatrix(storeManager, shipmentManager, pheromoneMatrix);
        ants.push_back(move(ant));
    }

    // calculate the total distance of the routes and other things
    for(const auto& ant : ants) {
        for(const auto& route : ant->getRoutes()) {
            route->calculateTotalDistance(distanceDurationManager);
            route->calculateTotalDuration(distanceDurationManager);
            route->calculateTotalPaletteCount();

            // Assigning the pheromone value to the route
            int totalDistance = route->getTotalDistance();
            int totalStoresCount = route->getStores().size();
            // Convert one of the variables to double for floating-point division
            double pheromone = static_cast<double>(totalStoresCount) / totalDistance;
            route->setPheromone(pheromone);
        }
    }

    // calculate the total distance of the routes and other things for the ants
    for(const auto& ant : ants) {
        for(const auto& route : ant->getRoutes()) {
            ant->setTotalDistance(ant->getTotalDistance() + route->getTotalDistance());
            ant->setTotalDuration(ant->getTotalDuration() + route->getTotalDuration());
            ant->setTotalPaletteCount(ant->getTotalPaletteCount() + route->getTotalPaletteCount());
            ant->calculateFitnessValue();
            ant->setPheromone(static_cast<double>(shipmentManager.getShipments().size()) / ant.get()->getTotalDistance());
            
        }
    }
    // sort the ants by fitness value
    sortAntsByFitnessValue();
}

// generate route based on pheromone matrix for ant
void Ant::generateRouteBasedOnPheromoneMatrix(const StoreManager& storeManager, const ShipmentManager& shipmentManager, PheromoneMatrix& pheromoneMatrix) {
    vector<Shipment> remainedShipments;
    for(const auto& shipment : shipmentManager.getShipments()) { if(shipment->getNoOfShipments() > 0) { remainedShipments.push_back(*shipment); } }
    int currentCapacity;

    // while there are still shipments to be delivered
    while(!remainedShipments.empty()) {
        Route route;
        currentCapacity = 0;

        Shipment currentShipment = shipmentManager.getRandShipment(remainedShipments);
        string currentStoreId = currentShipment.getStoreId();
        // cout << "-------------------------------------------" << endl;
        // cout << "remainedShipments capacity: " << remainedShipments.size() << endl;
        // cout << "-------------------------------------------" << endl;

        // while the current capacity is less than the vehicle capacity and there are still shipments to be delivered
        while(currentCapacity < this->vehicleCapacity && !remainedShipments.empty()) {
            // capacity is exceeded when the current shipment is added
            if(currentCapacity < this->vehicleCapacity && currentCapacity + currentShipment.getNoOfShipments() >= this->vehicleCapacity) {
                currentCapacity += currentShipment.getNoOfShipments();
                shared_ptr<Shipment> currentShipmentPtr = make_shared<Shipment>(currentShipment);
                Store store = storeManager.getStoreById(currentShipment.getStoreId());
                shared_ptr<Store> storePtr = make_shared<Store>(store);

                route.addVisitPoint(storePtr, currentShipmentPtr);
                if(remainedShipments.size() == 1) {
                    this->addRoute(route);
                    this->numberOfVehicleUsed++;
                    return;
                }
                // remove the shipment from remainedShipments
                shipmentManager.removeShipment(remainedShipments, currentShipment.getShipmentId());
                
                // currentStoreId = pheromoneMatrix.getNextStoreByPheromone(currentStoreId, remainedShipments);
                // currentShipment = shipmentManager.getShipmentByStoreId(currentStoreId);
                // cout << "****** In if statement" << endl;
            } 
            // capacity is not exceeded when the current shipment is added
            else {
                // cout << "Current capacity before addition: " << currentCapacity << endl;
                currentCapacity += currentShipment.getNoOfShipments();
                // cout << "Current capacity after addition: " << currentCapacity << endl;

                shared_ptr<Shipment> currentShipmentPtr = make_shared<Shipment>(currentShipment);
                Store store = storeManager.getStoreById(currentShipment.getStoreId());
                shared_ptr<Store> storePtr = make_shared<Store>(store);
                route.addVisitPoint(storePtr, currentShipmentPtr);

                // remove the shipment from remainedShipments
                // cout << "Number of shipments before removal: " << remainedShipments.size() << endl;
                if(remainedShipments.size() == 1) {
                    this->addRoute(route);
                    this->numberOfVehicleUsed++;
                    return;
                }
                shipmentManager.removeShipment(remainedShipments, currentShipment.getShipmentId());
                // cout << "Number of shipments after removal: " << remainedShipments.size() << endl;
                // get the next store based on the pheromone matrix
                currentStoreId = pheromoneMatrix.getNextStoreByPheromone(currentStoreId, remainedShipments);
                currentShipment = shipmentManager.getShipmentByStoreId(currentStoreId);
            }
            
        }
        this->addRoute(route);
        this->numberOfVehicleUsed++;
    }
}

/* ------------------------------------HEURISTIC MATRIX ------------------------------------ */

HeuristicMatrix::HeuristicMatrix(const StoreManager& storeManager, DistanceDurationManager& distanceDurationManager, const ShipmentManager& shipmentManager) {
    // push the stores with shipment
    for(const auto& shipment : shipmentManager.getShipments()) {
        if(shipment->getNoOfShipments() > 0) {
            Store store = storeManager.getStoreById(shipment->getStoreId());
            stores.push_back(store);
        }
    }

    // generate heuristic matrix with initial value 0
    for(int i = 0; i < stores.size(); i++) {
        vector<double> row;
        for(int j = 0; j < stores.size(); j++) {
            row.push_back(0.0);
        }
        heuristicMatrix.push_back(row);
    }

    // get distance between stores from distanceDurationManager using vector<unique_ptr<StoresDistancesDurations>>& getStoresDistancesDurations() { return storesDistancesDurations; }
    for(const auto& distanceDuration : distanceDurationManager.getStoresDistancesDurations()) {
        // if current and next id is in the stores vector
        for(const auto& store : stores) {
            if(distanceDuration->getCurrentId() == store.getStoreId()) {
                for(const auto& store2 : stores) {
                    if(distanceDuration->getNextId() == store2.getStoreId()) {
                        // assign the distance to the heuristic matrix
                        for(int i = 0; i < stores.size(); i++) {
                            if(stores[i].getStoreId() == distanceDuration->getCurrentId()) {
                                for(int j = 0; j < stores.size(); j++) {
                                    if(stores[j].getStoreId() == distanceDuration->getNextId()) {
                                        double distance = stod(distanceDuration->getDistance());
                                        heuristicMatrix[i][j] = distance;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }    
}

// display stores
void HeuristicMatrix::displayStores() const {
    for(const auto& store : stores) {
        cout << store.getStoreId() << endl;
    }
    cout << endl;
}

// display heuristic matrix
void HeuristicMatrix::displayHeuristicMatrix() const {
    cout << "------------------------------------------------- HEURISTIC MATRIX -------------------------------------------------" << endl;
    // print storeIds with first 7 characters
    for(const auto& store : stores) {
        cout << store.getStoreId().substr(3, 6) << " ";
    }
    cout << endl;

    // print heuristic matrix
    int i = 0;
    for(const auto& row : heuristicMatrix) {
        for(const auto& element : row) {
            cout << fixed << setprecision(3) << setw(6) << element << " ";
        }
        cout << stores[i++].getStoreId().substr(3, 6) << endl;
    }

    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
}