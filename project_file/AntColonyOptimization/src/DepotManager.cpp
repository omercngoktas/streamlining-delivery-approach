#include <iostream>
#include "../include/DepotManager.h"
using namespace std;


DepotManager::DepotManager() {
    cout << "DepotManager created" << endl;
}

DepotManager::~DepotManager() {
    cout << "DepotManager destroyed" << endl;
}

Depot::Depot() {
    cout << "Depot created" << endl;
}

Depot::~Depot() {
    cout << "Depot destroyed" << endl;
}