#include <iostream>
#include "../include/VehicleManager.h"
using namespace std;


VehicleManager::VehicleManager() {
    cout << "VehicleManager created" << endl;
}

VehicleManager::~VehicleManager() {
    cout << "VehicleManager destroyed" << endl;
}

Vehicle::Vehicle() {
    cout << "Vehicle created" << endl;
}

Vehicle::~Vehicle() {
    cout << "Vehicle destroyed" << endl;
}