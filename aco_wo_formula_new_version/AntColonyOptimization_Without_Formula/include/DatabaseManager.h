#ifndef MYSQLCONNECTOR_H
#define MYSQLCONNECTOR_H

#include <iostream>
#include <vector>
#include <string>

#include "StoreManager.h"
#include "DepotManager.h"
#include "ShipmentManager.h"
#include "VehicleManager.h"
#include "DistanceDurationManager.h"

#include "/opt/homebrew/Cellar/mysql-client/8.3.0/include/mysql/mysql.h"

class MySQLConnector {
    public:
        // Constructor now takes connection details as parameters
        MySQLConnector(const char* host, const char* user, const char* password, const char* database, unsigned int port = 3306);
        ~MySQLConnector();
        
        // Connects to the MySQL database
        bool connect();
        // Disconnects from the database
        void disconnect();
        // Executes a given SQL query and prints the results
        bool query(const char* sql);

        void fetchStores(StoreManager& storeManager);
        void fetchDepot(DepotManager& depotManager);
        void fetchVehicles(VehicleManager& vehicleManager);
        void fetchStoresDistancesDurations(DistanceDurationManager& distanceDurationManager, int year, int month, int day);
        void fetchDepotDistancesDurations(DistanceDurationManager& distanceDurationManager, int year, int month, int day);
        void fetchShipments(ShipmentManager& shipmentManager, int year, int month, int day);

    private:
        MYSQL* conn; // Pointer to MYSQL structure, represents a connection to MySQL
        const char* host;
        const char* user;
        const char* password;
        const char* database;
        unsigned int port;
};

#endif // MYSQLCONNECTOR_H