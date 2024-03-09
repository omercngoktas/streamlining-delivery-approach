#ifndef DISTANCE_DURATION_MANAGER
#define DISTANCE_DURATION_MANAGER

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

class StoresDistancesDurations {
    public:
        StoresDistancesDurations() = default;
        StoresDistancesDurations(const string& st_dist_dura_id, const string& date, const string& current_id, const double& current_latitude, const double& current_longitude, const string& duration, const string& distance, const string& next_id, const double& next_latitude, const double& next_longitude);
        ~StoresDistancesDurations() = default;

        // Setters
        void setStDistDuraId(const string& st_dist_dura_id);
        void setDate(const string& date);
        void setCurrentId(const string& current_id);
        void setCurrentLatitude(const double& current_latitude);
        void setCurrentLongitude(const double& current_longitude);
        void setDuration(const string& duration);
        void setDistance(const string& distance);
        void setNextId(const string& next_id);
        void setNextLatitude(const double& next_latitude);
        void setNextLongitude(const double& next_longitude);

        // Getters
        string getStDistDuraId() const;
        string getDate() const;
        string getCurrentId() const;
        double getCurrentLatitude() const;
        double getCurrentLongitude() const;
        string getDuration() const;
        string getDistance() const;
        string getNextId() const;
        double getNextLatitude() const;
        double getNextLongitude() const;

    private:
        string st_dist_dura_id;
        string date;
        string current_id;
        double current_latitude;
        double current_longitude;
        string duration;
        string distance;
        string next_id;
        double next_latitude;
        double next_longitude;
};

class DepotsDistancesDurations {
    public:
        DepotsDistancesDurations() = default;
        DepotsDistancesDurations(const string& dp_dist_dura_id, const string& date, const string& depot_id, const double& depot_latitude, const double& depot_longitude, const string& duration, const string& distance, const string& next_id, const double& next_latitude, const double& next_longitude);
        ~DepotsDistancesDurations() = default;

        // Setters
        void setDpDistDuraId(const string& dp_dist_dura_id);
        void setDate(const string& date);
        void setDepotId(const string& depot_id);
        void setDepotLatitude(const double& depot_latitude);
        void setDepotLongitude(const double& depot_longitude);
        void setDuration(const string& duration);
        void setDistance(const string& distance);
        void setNextId(const string& next_id);
        void setNextLatitude(const double& next_latitude);
        void setNextLongitude(const double& next_longitude);

        // Getters
        string getDpDistDuraId() const;
        string getDate() const;
        string getDepotId() const;
        double getDepotLatitude() const;
        double getDepotLongitude() const;
        string getDuration() const;
        string getDistance() const;
        string getNextId() const;
        double getNextLatitude() const;
        double getNextLongitude() const;

    private:
        string dp_dist_dura_id;
        string date;
        string depot_id;
        double depot_latitude;
        double depot_longitude;
        string duration;
        string distance;
        string next_id;
        double next_latitude;
        double next_longitude;
};

class DistanceDurationManager {
    public:
        DistanceDurationManager() = default;
        ~DistanceDurationManager() = default;
        void addStoresDistancesDurations(const string& st_dist_dura_id, const string& date, const string& current_id, const double& current_latitude, const double& current_longitude, const string& duration, const string& distance, const string& next_id, const double& next_latitude, const double& next_longitude);
        void addDepotsDistancesDurations(const string& dp_dist_dura_id, const string& date, const string& depot_id, const double& depot_latitude, const double& depot_longitude, const string& duration, const string& distance, const string& next_id, const double& next_latitude, const double& next_longitude);
        void displayStoresDistancesDurations();
        void displayDepotsDistancesDurations();
        vector<unique_ptr<StoresDistancesDurations>>& getStoresDistancesDurations();

    private:
        vector<unique_ptr<StoresDistancesDurations>> storesDistancesDurations;
        vector<unique_ptr<DepotsDistancesDurations>> depotsDistancesDurations;
};

#endif