#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

struct Settings {
    std::string temperatureUnit = "celcius";  // Default to Celsius
    std::string windSpeedUnit = "kmh";
    std::string precipitationUnit = "mm";
    std::string timeFormat = "iso8601";
    std::string pastDays = "0";
    std::string forecastDays = "7";
    std::string startDate = "";
    std::string endDate = "";
    std::string timeZone = "GMT";
    // Default to kilometers per hour
    // Add other settings here as needed
};

#endif // SETTINGS_H