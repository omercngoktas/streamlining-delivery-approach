#include "../include/DistanceDurationManager.h"

StoresDistancesDurations::StoresDistancesDurations(const string& st_dist_dura_id, const string& date, const string& current_id, const double& current_latitude, const double& current_longitude, const string& duration, const string& distance, const string& next_id, const double& next_latitude, const double& next_longitude)
    : st_dist_dura_id(st_dist_dura_id), date(date), current_id(current_id), current_latitude(current_latitude), current_longitude(current_longitude), duration(duration), distance(distance), next_id(next_id), next_latitude(next_latitude), next_longitude(next_longitude) {}

DepotsDistancesDurations::DepotsDistancesDurations(const string& dp_dist_dura_id, const string& date, const string& depot_id, const double& depot_latitude, const double& depot_longitude, const string& duration, const string& distance, const string& next_id, const double& next_latitude, const double& next_longitude)
    : dp_dist_dura_id(dp_dist_dura_id), date(date), depot_id(depot_id), depot_latitude(depot_latitude), depot_longitude(depot_longitude), duration(duration), distance(distance), next_id(next_id), next_latitude(next_latitude), next_longitude(next_longitude) {}

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

// get depot distances durations by depot id and next id
DepotsDistancesDurations DistanceDurationManager::getDepotDistDuraById(const string& depot_id, const string& next_id) const {
    for (const auto& ddd : depotsDistancesDurations) {
        if (ddd->getDepotId() == depot_id && ddd->getNextId() == next_id) {
            return *ddd;
        }
    }
    return DepotsDistancesDurations();
}

// get store distances durations by current id and next id
StoresDistancesDurations DistanceDurationManager::getStoreDistDuraById(const string& current_id, const string& next_id) const {
    for (const auto& sdd : storesDistancesDurations) {
        if (sdd->getCurrentId() == current_id && sdd->getNextId() == next_id) {
            return *sdd;
        }
    }
    return StoresDistancesDurations();
}