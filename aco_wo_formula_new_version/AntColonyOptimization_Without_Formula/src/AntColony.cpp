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
    double initialPheromone = 1.0;

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
            row.push_back(initialPheromone);
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
void PheromoneMatrix::buildPheromoneMatrix(Ant& ant) {
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
            // find index for store1 and store2 in the pheromone matrix
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

// with the given storeId, returns the next storeId with the highest pheromone
string PheromoneMatrix::getNextStoreByPheromone(string storeId, vector<Shipment>& remainedShipments) {
    int storeIndex = -1;

    for(int i = 0; i < shipments.size(); i++) {
        if(shipments[i].getStoreId() == storeId) {
            storeIndex = i;
            break;
        }
    }

    double maxPheromone = 0.0;
    string nextStoreId = "";
    for(int i = 0; i < pheromoneMatrix[storeIndex].size(); i++) {
        if(i == storeIndex) {
            continue;
        }
        
        for(const auto& shipment : remainedShipments) {
            if(shipments[i].getStoreId() == shipment.getStoreId()) {
                if(pheromoneMatrix[storeIndex][i] > maxPheromone) {
                    maxPheromone = pheromoneMatrix[storeIndex][i];
                    nextStoreId = shipments[i].getStoreId();
                }
            }
        }
    }
    return nextStoreId;
}

// generate routes based on pheromone matrix for ant colony
void AntColony::generateRoutesBasedOnPheromoneMatrix(const StoreManager& storeManager, const ShipmentManager& shipmentManager, PheromoneMatrix& pheromoneMatrix, const DistanceDurationManager& distanceDurationManager, const HeuristicMatrix& heuristicMatrix) {
    for(int i = 0; i < numOfAnts; i++) {
        auto ant = std::make_unique<Ant>(vehicleCapacity);
        ant->generateRouteBasedOnPheromoneMatrix(storeManager, shipmentManager, pheromoneMatrix, heuristicMatrix);
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
void Ant::generateRouteBasedOnPheromoneMatrix(const StoreManager& storeManager, const ShipmentManager& shipmentManager, PheromoneMatrix& pheromoneMatrix, const HeuristicMatrix& heuristicMatrix) {
    vector<Shipment> remainedShipments;
    for(const auto& shipment : shipmentManager.getShipments()) { if(shipment->getNoOfShipments() > 0) { remainedShipments.push_back(*shipment); } }
    int currentCapacity;

    // while there are still shipments to be delivered
    while(!remainedShipments.empty()) {
        Route route;
        currentCapacity = 0;

        Shipment currentShipment = shipmentManager.getRandShipment(remainedShipments);
        string currentStoreId = currentShipment.getStoreId();

        // while the current capacity is less than the vehicle capacity and there are still shipments to be delivered
        while(currentCapacity < this->vehicleCapacity && !remainedShipments.empty()) {
            // capacity is exceeded when the current shipment is added
            if(currentCapacity < this->vehicleCapacity && currentCapacity + currentShipment.getNoOfShipments() > this->vehicleCapacity) {
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
                
            } 
            // capacity is not exceeded when the current shipment is added
            else {
                currentCapacity += currentShipment.getNoOfShipments();

                shared_ptr<Shipment> currentShipmentPtr = make_shared<Shipment>(currentShipment);
                Store store = storeManager.getStoreById(currentShipment.getStoreId());
                shared_ptr<Store> storePtr = make_shared<Store>(store);
                route.addVisitPoint(storePtr, currentShipmentPtr);

                // remove the shipment from remainedShipments
                if(remainedShipments.size() == 1) {
                    this->addRoute(route);
                    this->numberOfVehicleUsed++;
                    return;
                }
                shipmentManager.removeShipment(remainedShipments, currentShipment.getShipmentId());
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

// Evaporate pheromone matrix
void PheromoneMatrix::evaporatePheromoneMatrix(const AntColony& antColonyPheromone) {
    // Evaporation formula: τij ← (1−ρ) * τij
    // pheromoneMatrix[i][j] = (1 - p) * pheromoneMatrix[i][j]
    double p = 0.1;
    for(int i = 0; i < pheromoneMatrix.size(); i++) {
        for(int j = 0; j < pheromoneMatrix[i].size(); j++) {
            pheromoneMatrix[i][j] = (1 - p) * pheromoneMatrix[i][j];
        }
    }
}

// Deposit pheromone to the pheromone matrix
void PheromoneMatrix::pheromoneDeposition(AntColony& antColonyPheromone) {
    // Deposition: order of the current ant / fitness value of the current ant
    // If there are 20 ants, first ant will update the pheromone matrix with the value:
    // x + 1 / fitness value of the first ant, where x is the current value in the pheromone matrix
    // for the second ant x + 2 / fitness value of the second ant, and so on.

    for(int i = 0; i < antColonyPheromone.getAnts().size(); i++) {
        double fitnessValue = antColonyPheromone.getAnt(i).getFitnessValue();
        for(const auto& route : antColonyPheromone.getAnt(i).getRoutes()) {
            for(int j = 0; j < route->getStores().size() - 1; j++) {
                int store1Index = -1;
                int store2Index = -1;

                for(int k = 0; k < shipments.size(); k++) {
                    if(shipments[k].getStoreId() == route->getStores()[j]->getStoreId()) {
                        store1Index = k;
                    }
                    if(shipments[k].getStoreId() == route->getStores()[j + 1]->getStoreId()) {
                        store2Index = k;
                    }
                }
                
                if(store1Index != -1 && store2Index != -1) {
                    pheromoneMatrix[store1Index][store2Index] += (i + 1) / fitnessValue;
                    pheromoneMatrix[store2Index][store1Index] += (i + 1) / fitnessValue;
                }
            }
        }
    }
}

void Ant::generateBestRoute(StoreManager& storeManager, ShipmentManager& shipmentManager, DepotManager& depotManager, DistanceDurationManager& distanceDurationManager) {
    // from the current routes, best order of the stores should be found
    // first of all, closest store to the depot should be found and added to the route
    // then, the closest store to the last store should be found and added to the route
    // this process should be repeated until all the stores are visited

    // get the depot
    Depot depot = depotManager.getDepot();
    vector<Route> bestRoutes;

    for(int i = 0; i < this->routes.size(); i++) {
        Store closestStoreToDepot;
        double minDistance = 999999999.0;
        
        for(int j = 0; j < this->routes[i]->getStores().size(); j++) {
            double distance = depot.getDistanceToStore(this->routes[i]->getStores()[j]->getLatitude(), this->routes[i]->getStores()[j]->getLongitude());
            if(distance < minDistance) {
                minDistance = distance;
                closestStoreToDepot = storeManager.getStoreById(this->routes[i]->getStores()[j]->getStoreId());
            }
        }

        // add the closest store to the depot to the route
        shared_ptr<Store> closestStoreToDepotPtr = make_shared<Store>(closestStoreToDepot);
        shared_ptr<Shipment> shipment = make_shared<Shipment>(shipmentManager.getShipmentByStoreId(closestStoreToDepot.getStoreId()));
        Route route;
        route.addVisitPoint(closestStoreToDepotPtr, shipment);
        bestRoutes.push_back(route);
        this->removeStoreFromShipment(closestStoreToDepot.getStoreId());
    }

    // finding closest store to the stores in order
    for(int i = 0; i < this->routes.size(); i++) {
        while(this->routes[i]->getStores().size() > 0) {
            Store lastStore = *bestRoutes[i].getStores()[bestRoutes[i].getStores().size() - 1];
            Store closestStore;
            double minDistance = 999999999.0;

            for(int j = 0; j < this->routes[i]->getStores().size(); j++) {
                double distance = lastStore.getDistanceToStore(this->routes[i]->getStores()[j]->getLatitude(), this->routes[i]->getStores()[j]->getLongitude());
                if(distance < minDistance) {
                    minDistance = distance;
                    closestStore = storeManager.getStoreById(this->routes[i]->getStores()[j]->getStoreId());
                }
            }

            shared_ptr<Store> closestStorePtr = make_shared<Store>(closestStore);
            shared_ptr<Shipment> shipment = make_shared<Shipment>(shipmentManager.getShipmentByStoreId(closestStore.getStoreId()));
            Route route;
            route.addVisitPoint(closestStorePtr, shipment);
            bestRoutes[i].addVisitPoint(closestStorePtr, shipment);
            this->removeStoreFromShipment(closestStore.getStoreId());
        }
    }

    for(auto& route : bestRoutes) {
        route.calculateTotalDistance(distanceDurationManager);
        route.calculateTotalDuration(distanceDurationManager);
        route.calculateTotalPaletteCount();
    }

    this->routes.clear();
    for(auto& route : bestRoutes) {
        this->addRoute(route);
    }
    
}

// remove store from shipment
void Ant::removeStoreFromShipment(string storeId) {
    for(int i = 0; i < this->routes.size(); i++) {
        for(int j = 0; j < this->routes[i]->getStores().size(); j++) {
            if(this->routes[i]->getStores()[j]->getStoreId() == storeId) {
                this->routes[i]->removeStoreFromRoute(storeId);
            }
        }
    }
}

// calculate the total distance of the ant
void Ant::calculateTotalRouteDistance(const DistanceDurationManager& distanceDurationManager) {
    int totalDistance = 0;
    for(const auto& route : routes) {
        totalDistance += route->getTotalDistance();
    }
    this->setTotalDistance(totalDistance);
}