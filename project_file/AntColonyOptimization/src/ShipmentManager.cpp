#include "../include/ShipmentManager.h"

// Constructor for Shipment
Shipment::Shipment(const string& shipment_id, const string& store_id, const string& date, const int& no_of_shipments)
    : shipment_id(shipment_id), store_id(store_id), date(date), no_of_shipments(no_of_shipments) {}

// Add a shipment
void ShipmentManager::addShipment(const string& shipment_id, const string& store_id, const string& date, const int& no_of_shipments) {
    shipments.push_back(make_unique<Shipment>(shipment_id, store_id, date, no_of_shipments));
    if (no_of_shipments > 0) {
        total_palette_count += no_of_shipments;
        number_of_stores_to_ship++;
    }
}

// Display the shipments
void ShipmentManager::displayShipments() {
    for (const auto& shipment : shipments) {
        cout << "Shipment ID: " << shipment->getShipmentId();
        cout << " Store ID: " << shipment->getStoreId();
        cout << " Date: " << shipment->getDate();
        cout << " Number of shipments: " << shipment->getNoOfShipments() << endl;
    }
    cout << "Total palette count: " << total_palette_count << endl;
    cout << "Number of stores to ship: " << number_of_stores_to_ship << endl;
}

// Get the shipment by store_id
Shipment& ShipmentManager::getShipmentByStoreId(const string& store_id) {
    for (const auto& shipment : shipments) {
        if (shipment->getStoreId() == store_id) {
            return *shipment;
        }
    }
    throw invalid_argument("Store ID not found");
}

// Get the palette count by store_id
int ShipmentManager::getPaletteCountByStoreId(string store_id) const {
    for (const auto& shipment : shipments) {
        if (shipment->getStoreId() == store_id) {
            return shipment->getNoOfShipments();
        }
    }
    throw invalid_argument("Store ID not found");
}

// Display the shipment
void Shipment::displayShipment() const {
    cout << "Shipment ID: " << shipment_id;
    cout << " Store ID: " << store_id;
    cout << " Date: " << date;
    cout << " Number of shipments: " << no_of_shipments << endl;
}

// Get a random shipment from the given vector
Shipment ShipmentManager::getRandShipment(vector<Shipment>& currentShipments) const {
    int randomIndex = rand() % currentShipments.size();
    return currentShipments[randomIndex];
}

// Remove a shipment from the given vector with the given shipment_id
void ShipmentManager::removeShipment(vector<Shipment>& shipments, string shipment_id) const {
    for (auto it = shipments.begin(); it != shipments.end(); it++) {
        if (it->getShipmentId() == shipment_id) {
            // cout << "Shipment with ID " << shipment_id << " removed" << endl;
            shipments.erase(it);
            return;
        }
    }
    throw invalid_argument("Shipment with ID " + shipment_id + " not found");
}

// Get the best suitable shipment for the vehicle capacity. If there is no suitable shipment, return null
Shipment ShipmentManager::getBestShipment(vector<Shipment>& currentShipments, int vehicleCapacity) const {
    int bestSuitableShipmentIndex = 0;
    int bestSuitableShipmentPaletteCount = 0;
    for (int i = 0; i < currentShipments.size(); i++) {
        if (currentShipments[i].getNoOfShipments() <= vehicleCapacity && currentShipments[i].getNoOfShipments() > bestSuitableShipmentPaletteCount) {
            bestSuitableShipmentIndex = i;
            bestSuitableShipmentPaletteCount = currentShipments[i].getNoOfShipments();
        }
    }
    // If there is no suitable shipment, return null
    if (bestSuitableShipmentPaletteCount == 0) {
        // cout << "No suitable shipment found" << endl;
        return Shipment("", "", "", 0);
    }
    return currentShipments[bestSuitableShipmentIndex];
}