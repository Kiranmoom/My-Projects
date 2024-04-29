#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <vector>
#include <string>
#include <curl/curl.h>
#include "nlohmann/json.hpp"
#include "SettingsManager.h"  // Include SettingsManager

class ApiHandler {
public:
    ApiHandler(SettingsManager& settingsManager);  // Constructor now takes SettingsManager reference
    ~ApiHandler();
    nlohmann::json fetchWeatherData(double latitude, double longitude, const std::vector<std::string>& parameters, const std::string& startDate = "", const std::string& endDate = "");

private:
    SettingsManager& settingsManager;  // Reference to SettingsManager to use settings
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer);
};

#endif // APIHANDLER_H