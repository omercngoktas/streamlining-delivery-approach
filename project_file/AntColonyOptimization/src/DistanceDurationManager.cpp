#include "../include/DistanceDurationManager.h"

StoresDistancesDurations::StoresDistancesDurations(const string& st_dist_dura_id, const string& date, const string& current_id, const double& current_latitude, const double& current_longitude, const string& duration, const string& distance, const string& next_id, const double& next_latitude, const double& next_longitude)
    : st_dist_dura_id(st_dist_dura_id), date(date), current_id(current_id), current_latitude(current_latitude), current_longitude(current_longitude), duration(duration), distance(distance), next_id(next_id), next_latitude(next_latitude), next_longitude(next_longitude) {}

// Setters
void StoresDistancesDurations::setStDistDuraId(const string& st_dist_dura_id) { this->st_dist_dura_id = st_dist_dura_id; }
void StoresDistancesDurations::setDate(const string& date) { this->date = date; }
void StoresDistancesDurations::setCurrentId(const string& current_id) { this->current_id = current_id; }
void StoresDistancesDurations::setCurrentLatitude(const double& current_latitude) { this->current_latitude = current_latitude; }
void StoresDistancesDurations::setCurrentLongitude(const double& current_longitude) { this->current_longitude = current_longitude; }
void StoresDistancesDurations::setDuration(const string& duration) { this->duration = duration; }
void StoresDistancesDurations::setDistance(const string& distance) { this->distance = distance; }
void StoresDistancesDurations::setNextId(const string& next_id) { this->next_id = next_id; }
void StoresDistancesDurations::setNextLatitude(const double& next_latitude) { this->next_latitude = next_latitude; }
void StoresDistancesDurations::setNextLongitude(const double& next_longitude) { this->next_longitude = next_longitude; }
// Getters
string StoresDistancesDurations::getStDistDuraId() const { return st_dist_dura_id; }
string StoresDistancesDurations::getDate() const { return date; }
string StoresDistancesDurations::getCurrentId() const { return current_id; }
double StoresDistancesDurations::getCurrentLatitude() const { return current_latitude; }
double StoresDistancesDurations::getCurrentLongitude() const { return current_longitude; }
string StoresDistancesDurations::getDuration() const { return duration; }
string StoresDistancesDurations::getDistance() const { return distance; }
string StoresDistancesDurations::getNextId() const { return next_id; }
double StoresDistancesDurations::getNextLatitude() const { return next_latitude; }
double StoresDistancesDurations::getNextLongitude() const { return next_longitude; }

DepotsDistancesDurations::DepotsDistancesDurations(const string& dp_dist_dura_id, const string& date, const string& depot_id, const double& depot_latitude, const double& depot_longitude, const string& duration, const string& distance, const string& next_id, const double& next_latitude, const double& next_longitude)
    : dp_dist_dura_id(dp_dist_dura_id), date(date), depot_id(depot_id), depot_latitude(depot_latitude), depot_longitude(depot_longitude), duration(duration), distance(distance), next_id(next_id), next_latitude(next_latitude), next_longitude(next_longitude) {}

// Setters
void DepotsDistancesDurations::setDpDistDuraId(const string& dp_dist_dura_id) { this->dp_dist_dura_id = dp_dist_dura_id; }      
void DepotsDistancesDurations::setDate(const string& date) { this->date = date; }
void DepotsDistancesDurations::setDepotId(const string& depot_id) { this->depot_id = depot_id; }
void DepotsDistancesDurations::setDepotLatitude(const double& depot_latitude) { this->depot_latitude = depot_latitude; }
void DepotsDistancesDurations::setDepotLongitude(const double& depot_longitude) { this->depot_longitude = depot_longitude; }
void DepotsDistancesDurations::setDuration(const string& duration) { this->duration = duration; }
void DepotsDistancesDurations::setDistance(const string& distance) { this->distance = distance; }
void DepotsDistancesDurations::setNextId(const string& next_id) { this->next_id = next_id; }
void DepotsDistancesDurations::setNextLatitude(const double& next_latitude) { this->next_latitude = next_latitude; }
void DepotsDistancesDurations::setNextLongitude(const double& next_longitude) { this->next_longitude = next_longitude; }
// Getters
string DepotsDistancesDurations::getDpDistDuraId() const { return dp_dist_dura_id; }
string DepotsDistancesDurations::getDate() const { return date; }
string DepotsDistancesDurations::getDepotId() const { return depot_id; }
double DepotsDistancesDurations::getDepotLatitude() const { return depot_latitude; }
double DepotsDistancesDurations::getDepotLongitude() const { return depot_longitude; }
string DepotsDistancesDurations::getDuration() const { return duration; }
string DepotsDistancesDurations::getDistance() const { return distance; }
string DepotsDistancesDurations::getNextId() const { return next_id; }
double DepotsDistancesDurations::getNextLatitude() const { return next_latitude; }
double DepotsDistancesDurations::getNextLongitude() const { return next_longitude; }

// adding stores distances durations to the vector
void DistanceDurationManager::addStoresDistancesDurations(const string& st_dist_dura_id, const string& date, const string& current_id, const double& current_latitude, const double& current_longitude, const string& duration, const string& distance, const string& next_id, const double& next_latitude, const double& next_longitude) {
    storesDistancesDurations.push_back(make_unique<StoresDistancesDurations>(st_dist_dura_id, date, current_id, current_latitude, current_longitude, duration, distance, next_id, next_latitude, next_longitude));
}
// adding depots distances durations to the vector
void DistanceDurationManager::addDepotsDistancesDurations(const string& dp_dist_dura_id, const string& date, const string& depot_id, const double& depot_latitude, const double& depot_longitude, const string& duration, const string& distance, const string& next_id, const double& next_latitude, const double& next_longitude) {
    depotsDistancesDurations.push_back(make_unique<DepotsDistancesDurations>(dp_dist_dura_id, date, depot_id, depot_latitude, depot_longitude, duration, distance, next_id, next_latitude, next_longitude));
}

// displaying stores distances durations
void DistanceDurationManager::displayStoresDistancesDurations() {
    for (const auto& sdd : storesDistancesDurations) {
        cout << sdd->getStDistDuraId() << " " << sdd->getDate() << " " << sdd->getCurrentId() << " " << sdd->getCurrentLatitude() << " " << sdd->getCurrentLongitude() << " " << sdd->getDuration() << " " << sdd->getDistance() << " " << sdd->getNextId() << " " << sdd->getNextLatitude() << " " << sdd->getNextLongitude() << endl;
    }
}
// displaying depots distances durations
void DistanceDurationManager::displayDepotsDistancesDurations() {
    for (const auto& ddd : depotsDistancesDurations) {
        cout << ddd->getDpDistDuraId() << " " << ddd->getDate() << " " << ddd->getDepotId() << " " << ddd->getDepotLatitude() << " " << ddd->getDepotLongitude() << " " << ddd->getDuration() << " " << ddd->getDistance() << " " << ddd->getNextId() << " " << ddd->getNextLatitude() << " " << ddd->getNextLongitude() << endl;
    }
}

// Getters
vector<unique_ptr<StoresDistancesDurations>>& DistanceDurationManager::getStoresDistancesDurations() { return storesDistancesDurations; }