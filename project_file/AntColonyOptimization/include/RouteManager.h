#ifndef ROUTE_MANAGER
#define ROUTE_MANAGER

#include "StoreManager.h"
#include "ShipmentManager.h"

using namespace std;

#include <iostream>
#include <memory>


class Route {
    public:
        Route() = default;
        ~Route() = default;
        void addVisitPoint(const Store& store, const Shipment& shipment);
        void displayRoute();
        // Getters
        int getTotalDistance() const { return this->totalDistance; }
        int getTotalDuration() const { return this->totalDuration; }
        int getTotalPaletteCount() const { return this->totalPaletteCount; }
        vector<unique_ptr<Store>>& getStores() { return this->stores; }
        vector<unique_ptr<Shipment>>& getShipments() { return this->shipments; }
        // Setters
        void setTotalDistance(int totalDistance) { this->totalDistance = totalDistance; }
        void setTotalDuration(int totalDuration) { this->totalDuration = totalDuration; }
        void setTotalPaletteCount(int totalPaletteCount) { this->totalPaletteCount = totalPaletteCount; }
        bool isRouteCompleted() const;
    private:
        vector<unique_ptr<Store>> stores;
        vector<unique_ptr<Shipment>> shipments;
        int totalDistance;
        int totalDuration;
        int totalPaletteCount;
};

#endif