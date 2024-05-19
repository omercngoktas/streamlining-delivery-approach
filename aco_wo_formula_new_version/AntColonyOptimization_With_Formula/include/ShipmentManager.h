#ifndef SHIPMENT_MANAGER
#define SHIPMENT_MANAGER

using namespace std;

#include <iostream>
#include <vector>
#include <memory>

class Shipment {
    public:
        Shipment() = default;
        Shipment(const string& shipment_id, const string& store_id, const string& date, const int& no_of_shipments);
        ~Shipment() = default;
        // Setters
        void setShipmentId(const string& shipment_id) { this->shipment_id = shipment_id; }
        void setStoreId(const string& store_id) { this->store_id = store_id; }
        void setDate(const string& date) { this->date = date; }
        void setNoOfShipments(const int& no_of_shipments) { this->no_of_shipments = no_of_shipments; }
        // Getters
        string getShipmentId() const { return shipment_id; }
        string getStoreId() const { return store_id; }
        string getDate() const { return date; }
        int getNoOfShipments() const { return no_of_shipments; }
        void displayShipment() const;
    private:
        string shipment_id;
        string store_id;
        string date;
        int no_of_shipments;
};

class ShipmentManager {
    public:
        ShipmentManager(int total_palette_count = 0, int number_of_stores_to_ship = 0) : total_palette_count(total_palette_count), number_of_stores_to_ship(number_of_stores_to_ship) {}
        ~ShipmentManager() = default;
        void addShipment(const string& shipment_id, const string& store_id, const string& date, const int& no_of_shipments);
        void displayShipments();
        const vector<unique_ptr<Shipment>>& getShipments() const { return shipments; }
        int getTotalPaletteCount() const { return total_palette_count; }
        int getNumberOfStoresToShip() const { return number_of_stores_to_ship; }
        Shipment& getShipmentByStoreId(const string& store_id);
        int getPaletteCountByStoreId(string store_id) const;
        Shipment getRandShipment(vector<Shipment>& currentShipments) const ;
        void removeShipment(vector<Shipment>& shipments, string shipment_id) const;
        Shipment getBestShipment(vector<Shipment>& currentShipments, int vehicleCapacity) const;
        Shipment getShipmentByShipmentId(string shipment_id) const;
        Shipment getShipmentByStoreId(string store_id) const;
        

    private:
        vector<unique_ptr<Shipment>> shipments;
        int total_palette_count;
        int number_of_stores_to_ship;
};

#endif