#include <iostream>
#include "LocationManager.h"
#include "SettingsManager.h"
#include "ApiHandler.h"

// Function to test LocationManager
void testLocationManager() {
    std::cout << "Testing LocationManager...\n";
    LocationManager locManager("test_locations.txt");
    LocationInfo locInfo{ "001", "TestLocation", 10.0, 20.0 };
    locManager.addLocation(locInfo);
    auto results = locManager.searchLocations("TestLocation");
    if (!results.empty() && results[0].locationName == "TestLocation") {
        std::cout << "PASS: Location added and retrieved successfully.\n";
    }
    else {
        std::cout << "FAIL: Location addition or retrieval failed.\n";
    }
}

// Function to test SettingsManager
void testSettingsManager() {
    std::cout << "Testing SettingsManager...\n";
    SettingsManager settingsManager;
    settingsManager.getSettings().temperatureUnit = "Fahrenheit";
    settingsManager.saveSettings();
    // Assuming reload or re-initialize to load saved settings
    SettingsManager newSettingsManager;
    newSettingsManager.loadSettings();
    if (newSettingsManager.getSettings().temperatureUnit == "Fahrenheit") {
        std::cout << "PASS: Settings changed and saved successfully.\n";
    }
    else {
        std::cout << "FAIL: Settings change or save failed.\n";
    }
}

