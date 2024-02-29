#include "../include/StoreManager.h"

StoreManager::StoreManager() {
    cout << "StoreManager created" << endl;
}

StoreManager::~StoreManager() {
    cout << "StoreManager destroyed" << endl;
}

Store::Store() {
    cout << "Store created" << endl;
}

Store::Store(const string& store_id, const string& store_location, const string& store_address, const double& latitude, const double& longitude, const string& depot_id) {
    this->store_id = store_id;
    this->store_location = store_location;
    this->store_address = store_address;
    this->latitude = latitude;
    this->longitude = longitude;
    this->depot_id = depot_id;
}

Store::~Store() {
    cout << "Store destroyed" << endl;
}

void StoreManager::readStores(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        // we will parse the csv file line by line however there are some rows which the value includes comma but it starts with " and ends with " so we need to handle this case
        while (getline(file, line)) {
            stringstream ss(line);
            string store_id, store_location, store_address;
            getline(ss, store_id, ',');
            getline(ss, store_location, ',');
            // Remove leading and trailing whitespaces
            /*store_id.erase(store_id.begin(), find_if(store_id.begin(), store_id.end(), [](int ch) {
                return !isspace(ch);
            }));
            store_id.erase(find_if(store_id.rbegin(), store_id.rend(), [](int ch) {
                return !isspace(ch);
            }).base(), store_id.end());
            store_location.erase(store_location.begin(), find_if(store_location.begin(), store_location.end(), [](int ch) {
                return !isspace(ch);
            }));
            store_location.erase(find_if(store_location.rbegin(), store_location.rend(), [](int ch) {
                return !isspace(ch);
            }).base(), store_location.end());*/

            getline(ss, store_address, ',');
            // Remove leading and trailing whitespaces
            store_address.erase(store_address.begin(), find_if(store_address.begin(), store_address.end(), [](int ch) {
                return !isspace(ch);
            }));
            store_address.erase(find_if(store_address.rbegin(), store_address.rend(), [](int ch) {
                return !isspace(ch);
            }).base(), store_address.end());



            // print all the handled values
            cout << "Store ID: " << store_id << ", Location: " << store_location << ", Address: " << store_address << endl;
        }
    }
}

void StoreManager::printStores() const {
    for (const auto& store : stores) {
        cout << "Store ID: " << store.store_id << ", Location: " << store.store_location << endl;
        // Print other store details as needed
    }
    cout << "Total stores: " << stores.size() << endl;
}
