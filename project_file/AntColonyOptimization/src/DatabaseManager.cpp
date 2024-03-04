#include "../include/DatabaseManager.h"
#include <iostream>

MySQLConnector::MySQLConnector(const char* host, const char* user, const char* password, const char* database, unsigned int port)
: conn(mysql_init(NULL)), host(host), user(user), password(password), database(database), port(port) {
    if (conn == NULL) {
        std::cerr << "MySQL initialization failed." << std::endl;
    } else {
        this->connect();
    }
}

MySQLConnector::~MySQLConnector() {
    this->disconnect();
}

bool MySQLConnector::connect() {
    if (!mysql_real_connect(conn, host, user, password, database, port, NULL, 0)) {
        std::cerr << "Connection to database failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    std::cout << "Successfully connected to database." << std::endl;
    return true;
}

void MySQLConnector::disconnect() {
    if (conn != NULL) {
        mysql_close(conn);
        conn = NULL;
        std::cout << "Disconnected from database." << std::endl;
    }
}

bool MySQLConnector::query(const char* sql) {
    if (mysql_query(conn, sql)) {
        std::cerr << "Query failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        std::cerr << "Failed to retrieve query results: " << mysql_error(conn) << std::endl;
        return false;
    }
    
    MYSQL_ROW row;
    unsigned int num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result))) {
        for(unsigned int i = 0; i < num_fields; i++) {
            std::cout << (row[i] ? row[i] : "NULL") << " ";
        }
        std::cout << std::endl;
    }

    mysql_free_result(result);
    return true;
}

void MySQLConnector::fetchStores(StoreManager& storeManager) {
    if (mysql_query(conn, "SELECT * FROM stores")) {
        std::cerr << "Query failed: " << mysql_error(conn) << std::endl;
        return;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        std::cerr << "Failed to retrieve query results: " << mysql_error(conn) << std::endl;
        return;
    }
    
    MYSQL_ROW row;
    unsigned int num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result))) {
        string store_id = row[0];
        string store_location = row[1];
        string store_address = row[2];
        double latitude = atof(row[3]);
        double longitude = atof(row[4]);
        string depot_id = row[5];
        storeManager.addStore(store_id, store_location, store_address, depot_id, latitude, longitude);
    }
    
    mysql_free_result(result);
}

void MySQLConnector::fetchDepot(DepotManager& depotManager) {
    if (mysql_query(conn, "SELECT * FROM depots")) {
        std::cerr << "Query failed: " << mysql_error(conn) << std::endl;
        return;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        std::cerr << "Failed to retrieve query results: " << mysql_error(conn) << std::endl;
        return;
    }
    
    MYSQL_ROW row;
    unsigned int num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result))) {
        string depot_id = row[0];
        string depot_location = row[1];
        string depot_address = row[2];
        double latitude = atof(row[3]);
        double longitude = atof(row[4]);
        depotManager.setDepot(depot_id, depot_location, depot_address, latitude, longitude);
    }
    
    mysql_free_result(result);
}

void MySQLConnector::fetchVehicles(VehicleManager& vehicleManager) {
    if (mysql_query(conn, "SELECT * FROM vehicles")) {
        std::cerr << "Query failed: " << mysql_error(conn) << std::endl;
        return;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        std::cerr << "Failed to retrieve query results: " << mysql_error(conn) << std::endl;
        return;
    }
    
    MYSQL_ROW row;
    unsigned int num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result))) {
        string vehicle_id = row[0];
        string depot_id = row[1];
        int palette_capacity = atoi(row[2]);
        string status = row[3];
        
        vehicleManager.addVehicle(vehicle_id, depot_id, status, palette_capacity);
    }
    
    mysql_free_result(result);
}