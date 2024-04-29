#include "ApiHandler.h"
#include <iostream>
#include <curl/curl.h>
#include <sstream>
#include <algorithm>

ApiHandler::ApiHandler(SettingsManager& settingsManager) : settingsManager(settingsManager) {
    curl_global_init(CURL_GLOBAL_ALL);
}

ApiHandler::~ApiHandler() {
    curl_global_cleanup();
}

nlohmann::json ApiHandler::fetchWeatherData(double latitude, double longitude, const std::vector<std::string>& parameters, const std::string& startDate, const std::string& endDate) {
    CURL* curl = curl_easy_init();
    std::string responseBuffer;

    if (curl) {
        std::string apiUrl = "https://api.open-meteo.com/v1/forecast?";
        apiUrl += "latitude=" + std::to_string(latitude);
        apiUrl += "&longitude=" + std::to_string(longitude);

        // Join parameters into a comma-separated list
        std::ostringstream joinedParams;
        std::copy(parameters.begin(), parameters.end(), std::ostream_iterator<std::string>(joinedParams, ","));
        std::string params = joinedParams.str();
        params.pop_back(); // Remove the last comma

        apiUrl += "&hourly=" + params;

        // Append start and end dates if they are provided
        if (!startDate.empty() && !endDate.empty()) {
            apiUrl += "&start_date=" + startDate;
            apiUrl += "&end_date=" + endDate;
        }

        // Include settings
        apiUrl += "&temperature_unit=" + settingsManager.getSettings().temperatureUnit;
        apiUrl += "&wind_speed_unit=" + settingsManager.getSettings().windSpeedUnit;
        apiUrl += "&precipitation_unit=" + settingsManager.getSettings().precipitationUnit;
        apiUrl += "&timeFormat_unit=" + settingsManager.getSettings().timeFormat;
        apiUrl += "&pastDays_unit=" + settingsManager.getSettings().pastDays;
        apiUrl += "&forecastDays_unit=" + settingsManager.getSettings().forecastDays;
        apiUrl += "&timeZone_unit=" + settingsManager.getSettings().timeZone;
        

        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Failed to perform HTTP request: " << curl_easy_strerror(res) << std::endl;
            return nlohmann::json();
        }

        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return nlohmann::json();
    }

    return nlohmann::json::parse(responseBuffer);
}

size_t ApiHandler::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t totalSize = size * nmemb;
    buffer->append((char*)contents, totalSize);
    return totalSize;
}
