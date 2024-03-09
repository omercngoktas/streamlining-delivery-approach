#include "../include/RouteManager.h"

// Adding a visit point to the route
void Route::addVisitPoint(const Store& store, const Shipment& shipment) {
    this->stores.push_back(make_unique<Store>(store));
    this->shipments.push_back(make_unique<Shipment>(shipment));
}

// Display the route
void Route::displayRoute() {
    cout << "Route: ";
    for (int i = 0; i < this->stores.size(); i++) {
        cout << this->stores[i]->getStoreId() << " -> ";
    }
    cout << "End" << endl;
}


bool Route::isRouteCompleted() const {
    return this->shipments.empty();
}

