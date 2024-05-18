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
        void setStDistDuraId(const string& st_dist_dura_id) { this->st_dist_dura_id = st_dist_dura_id; }
        void setDate(const string& date) { this->date = date; }
        void setCurrentId(const string& current_id) { this->current_id = current_id; }
        void setCurrentLatitude(const double& current_latitude) { this->current_latitude = current_latitude; }
        void setCurrentLongitude(const double& current_longitude) { this->current_longitude = current_longitude; }
        void setDuration(const string& duration) { this->duration = duration; }
        void setDistance(const string& distance) { this->distance = distance; }
        void setNextId(const string& next_id) { this->next_id = next_id; }
        void setNextLatitude(const double& next_latitude) { this->next_latitude = next_latitude; }
        void setNextLongitude(const double& next_longitude) { this->next_longitude = next_longitude; }
        // Getters
        string getStDistDuraId() const { return st_dist_dura_id; }
        string getDate() const { return date; }
        string getCurrentId() const { return current_id; }
        double getCurrentLatitude() const { return current_latitude; }
        double getCurrentLongitude() const { return current_longitude; }
        string getDuration() const { return duration; }
        string getDistance() const { return distance; }
        string getNextId() const { return next_id; }
        double getNextLatitude() const { return next_latitude; }
        double getNextLongitude() const { return next_longitude; }
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
        void setDpDistDuraId(const string& dp_dist_dura_id) { this->dp_dist_dura_id = dp_dist_dura_id; }
        void setDate(const string& date) { this->date = date; }
        void setDepotId(const string& depot_id) { this->depot_id = depot_id; }
        void setDepotLatitude(const double& depot_latitude) { this->depot_latitude = depot_latitude; }
        void setDepotLongitude(const double& depot_longitude) { this->depot_longitude = depot_longitude; }
        void setDuration(const string& duration) { this->duration = duration; }
        void setDistance(const string& distance) { this->distance = distance; }
        void setNextId(const string& next_id) { this->next_id = next_id; }
        void setNextLatitude(const double& next_latitude) { this->next_latitude = next_latitude; }
        void setNextLongitude(const double& next_longitude) { this->next_longitude = next_longitude; }
        // Getters
        string getDpDistDuraId() const { return dp_dist_dura_id; }
        string getDate() const { return date; }
        string getDepotId() const { return depot_id; }
        double getDepotLatitude() const { return depot_latitude; }
        double getDepotLongitude() const { return depot_longitude; }
        string getDuration() const { return duration; }
        string getDistance() const { return distance; }
        string getNextId() const { return next_id; }
        double getNextLatitude() const { return next_latitude; }
        double getNextLongitude() const { return next_longitude; }
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
        vector<unique_ptr<StoresDistancesDurations>>& getStoresDistancesDurations() { return storesDistancesDurations; }
        
        DepotsDistancesDurations getDepotDistDuraById(const string& depot_id, const string& next_id) const;
        StoresDistancesDurations getStoreDistDuraById(const string& current_id, const string& next_id) const;
        
    private:
        vector<unique_ptr<StoresDistancesDurations>> storesDistancesDurations;
        vector<unique_ptr<DepotsDistancesDurations>> depotsDistancesDurations;
};

#endif