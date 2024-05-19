#include "../include/RouteManager.h"

// Adding a visit point to the route
void Route::addVisitPoint(shared_ptr<Store> store, shared_ptr<Shipment> shipment) {
    this->stores.push_back(store);
    this->shipments.push_back(shipment);
}

void Route::displayRoute() {
    using std::cout;
    using std::endl;
    using std::left;
    using std::setw;

    // Calculate the total width based on the widths used in setw plus separators
    int totalWidth = 25 + 15 + 16 + 38 + 15 + 22 + 12 + 1; // Sum of all setw widths + separators('|' and spaces)
    
    // Function to print dashes
    auto printLine = [&]() {
        cout << "+"; // Start border
        for (int i = 1; i < totalWidth; i++) { // One less than totalWidth to account for starting '+'
            cout << "-";
        }
        cout << "+" << endl; // End border
    };

    printLine();
    cout << "| " << setw(25) << left << "Store ID"
         << "| " << setw(15) << "Store Latitude"
         << "| " << setw(16) << "Store Longitude"
         << "| " << setw(38) << "Shipment ID"
         << "| " << setw(15) << "Shipment Date"
         << "| " << setw(22) << "Shipment Palette Count |" << endl;
    printLine();
    for (int i = 0; i < this->stores.size(); i++) {
        cout << "| " << setw(25) << left << this->stores[i]->getStoreId()
             << "| " << setw(15) << this->stores[i]->getLatitude()
             << "| " << setw(16) << this->stores[i]->getLongitude()
             << "| " << setw(38) << this->shipments[i]->getShipmentId()
             << "| " << setw(15) << this->shipments[i]->getDate()
             << "| " << setw(22) << this->shipments[i]->getNoOfShipments() << " |" << endl;
    }
    cout << "| Total Distance: " << setw(10) << left << this->getTotalDistance() 
         << "Total Duration: " << setw(11) << this->getTotalDuration() 
         << "Total Palette Count: " << setw(5) << this->getTotalPaletteCount() << " |" << endl;
    printLine();
}


// calculate totalPaletteCount
void Route::calculateTotalPaletteCount() {
    int totalPaletteCount = 0;
    for (const auto& shipment : this->shipments) {
        totalPaletteCount += shipment->getNoOfShipments();
    }
    this->setTotalPaletteCount(totalPaletteCount);
}

// calculate totalDistance
void Route::calculateTotalDistance(const DistanceDurationManager& distanceDurationManager) {
    double totalDistance = 0;
    for (int i = 0; i < this->stores.size() - 1; i++) {
        string currentId = this->stores[i]->getStoreId();
        string nextId = this->stores[i + 1]->getStoreId();
        StoresDistancesDurations sdd = distanceDurationManager.getStoreDistDuraById(currentId, nextId);
        totalDistance += stod(sdd.getDistance());
    }
    this->setTotalDistance(totalDistance);

    // double totalDistance = 0;
    // for (int i = 0; i < this->stores.size() - 1; i++) {
    //     totalDistance += getDistanceBetweenStores(*this->stores[i], *this->stores[i + 1], distanceDurationManager);

    // }
    // this->setTotalDistance(totalDistance);
}



// calculate totalDuration
void Route::calculateTotalDuration(const DistanceDurationManager& distanceDurationManager) {
    double totalDuration = 0;
    for (int i = 0; i < this->stores.size() - 1; i++) {
        string currentId = this->stores[i]->getStoreId();
        string nextId = this->stores[i + 1]->getStoreId();
        StoresDistancesDurations sdd = distanceDurationManager.getStoreDistDuraById(currentId, nextId);
        totalDuration += stod(sdd.getDuration());
    }
    this->setTotalDuration(totalDuration);
}

// Get the distance between two stores
int Route::getDistanceBetweenStores(const Store& store1, const Store& store2, const DistanceDurationManager& distanceDurationManager) {
    string store1Id = store1.getStoreId();
    string store2Id = store2.getStoreId();
    StoresDistancesDurations sdd = distanceDurationManager.getStoreDistDuraById(store1Id, store2Id);
    return stoi(sdd.getDistance());
}

// Remove a store from the route
void Route::removeStoreFromRoute(string storeId) {
    for (int i = 0; i < this->stores.size(); i++) {
        if (this->stores[i]->getStoreId() == storeId) {
            this->stores.erase(this->stores.begin() + i);
            this->shipments.erase(this->shipments.begin() + i);
            break;
        }
    }
}