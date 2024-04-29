#ifndef LOCATIONINFO_H
#define LOCATIONINFO_H

#include <string>

struct LocationInfo {
    std::string locationId;
    std::string locationName;
    double latitude;
    double longitude;
};

#endif // LOCATIONINFO_H