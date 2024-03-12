#ifndef ROUTE_MANAGER
#define ROUTE_MANAGER

#include "StoreManager.h"
#include "ShipmentManager.h"
#include "DistanceDurationManager.h"
#include <iomanip> // Include for std::setw and std::left

using namespace std;

#include <iostream>
#include <memory>


class Route {
    public:
        Route() = default;
        ~Route() = default;
        void addVisitPoint(shared_ptr<Store> store, shared_ptr<Shipment> shipment);
        void displayRoute();
        // Getters
        int getTotalDistance() const { return this->totalDistance; }
        int getTotalDuration() const { return this->totalDuration; }
        int getTotalPaletteCount() const { return this->totalPaletteCount; }
        const vector<shared_ptr<Store>>& getStores() const { return this->stores; }
        const vector<shared_ptr<Shipment>>& getShipments() const { return this->shipments; }
        // Setters
        void setTotalDistance(int totalDistance) { this->totalDistance = totalDistance; }
        void setTotalDuration(int totalDuration) { this->totalDuration = totalDuration; }
        void setTotalPaletteCount(int totalPaletteCount) { this->totalPaletteCount = totalPaletteCount; }
        void calculateTotalDistance(const DistanceDurationManager& distanceDurationManager);
        void calculateTotalDuration(const DistanceDurationManager& distanceDurationManager);
        void calculateTotalPaletteCount();

    private:
        vector<shared_ptr<Store>> stores;
        vector<shared_ptr<Shipment>> shipments;
        double totalDistance;
        double totalDuration;
        int totalPaletteCount;
};

#endif