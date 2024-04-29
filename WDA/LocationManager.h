#ifndef LOCATION_MANAGER_H
#define LOCATION_MANAGER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include "LocationInfo.h"

class LocationManager {
public:
    LocationManager(const std::string& fileName);
    ~LocationManager();
    void addLocation(const LocationInfo& location);
    void removeLocationById(const std::string& locationId);
    void removeLocationByName(const std::string& locationName);
    void modifyLocation(const std::string& locationId, const LocationInfo& newLocation);
    std::vector<LocationInfo> searchLocations(const std::string& searchTerm);
    void displayLocation(const LocationInfo& location) const;
    void addFavorite(const std::string& locationId);
    void listFavorites() const;

private:
    std::vector<LocationInfo> locations;
    std::unordered_set<std::string> favoriteLocationIds;
    std::string fileName;
    void saveLocationsToFile();
    void loadLocationsFromFile();
};

#endif // LOCATION_MANAGER_H
