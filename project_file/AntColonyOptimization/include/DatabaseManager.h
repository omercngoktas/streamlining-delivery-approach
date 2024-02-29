#ifndef DATABASE_MANAGER
#define DATABASE_MANAGER

#include <iostream>

using namespace std;

class DatabaseManager {
    public:
        DatabaseManager();
        ~DatabaseManager();
        void connect();
        void disconnect();
};

#endif