#ifndef STORE_MANAGER
#define STORE_MANAGER

#include "Store.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class StoreManager {
    public:
        StoreManager();
        ~StoreManager();
        void readStores(const string& filename);
        void printStores() const;

    private:
        vector<Store> stores;

};

#endif