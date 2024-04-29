#include "LocationManager.h"

LocationManager::LocationManager(const std::string& fileName) : fileName(fileName) {
    loadLocationsFromFile();
}

LocationManager::~LocationManager() {
    saveLocationsToFile();
}

void LocationManager::addLocation(const LocationInfo& location) {
    locations.push_back(location);
    saveLocationsToFile();
}

void LocationManager::removeLocationById(const std::string& locationId) {
    locations.erase(std::remove_if(locations.begin(), locations.end(),
        [&locationId](const LocationInfo& loc) { return loc.locationId == locationId; }),
        locations.end());
    saveLocationsToFile();
}

void LocationManager::modifyLocation(const std::string& locationId, const LocationInfo& newLocation) {
    auto it = std::find_if(locations.begin(), locations.end(),
        [&locationId](const LocationInfo& loc) { return loc.locationId == locationId; });
    if (it != locations.end()) {
        *it = newLocation;
    }
    saveLocationsToFile();
}

std::vector<LocationInfo> LocationManager::searchLocations(const std::string& searchTerm) {
    std::vector<LocationInfo> foundLocations;
    for (const auto& location : locations) {
        if (location.locationName.find(searchTerm) != std::string::npos || location.locationId == searchTerm) {
            foundLocations.push_back(location);
        }
    }
    return foundLocations;
}

void LocationManager::saveLocationsToFile() {
    std::ofstream outputFile(fileName);
    for (const auto& location : locations) {
        outputFile << location.locationId << "," << location.locationName << ","
            << location.latitude << "," << location.longitude << std::endl;
    }
}

void LocationManager::loadLocationsFromFile() {
    std::ifstream inputFile(fileName);
    std::string line;
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        LocationInfo loc;
        getline(ss, loc.locationId, ',');
        getline(ss, loc.locationName, ',');
        ss >> loc.latitude;
        ss.ignore();
        ss >> loc.longitude;
        locations.push_back(loc);
    }
}

void LocationManager::displayLocation(const LocationInfo& location) const {
    std::cout << "ID: " << location.locationId
        << ", Name: " << location.locationName
        << ", Latitude: " << location.latitude
        << ", Longitude: " << location.longitude << std::endl;
}

void LocationManager::addFavorite(const std::string& locationId) {
    if (favoriteLocationIds.insert(locationId).second) {
        std::cout << "Location added to favorites.\n";
    }
    else {
        std::cout << "Location is already in favorites.\n";
    }
}

void LocationManager::listFavorites() const {
    if (favoriteLocationIds.empty()) {
        std::cout << "No favorite locations. Add some!\n";
    }
    else {
        for (const auto& id : favoriteLocationIds) {
            auto it = std::find_if(locations.begin(), locations.end(),
                [&id](const LocationInfo& loc) { return loc.locationId == id; });
            if (it != locations.end()) {
                displayLocation(*it);
            }
        }
    }
}