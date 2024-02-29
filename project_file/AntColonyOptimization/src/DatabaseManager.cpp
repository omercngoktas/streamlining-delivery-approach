#include "../include/DatabaseManager.h"

DatabaseManager::DatabaseManager() {
    cout << "DatabaseManager created" << endl;
}

DatabaseManager::~DatabaseManager() {
    cout << "DatabaseManager destroyed" << endl;
}

void DatabaseManager::connect() {
    cout << "DatabaseManager connected" << endl;
}

void DatabaseManager::disconnect() {
    cout << "DatabaseManager disconnected" << endl;
}
