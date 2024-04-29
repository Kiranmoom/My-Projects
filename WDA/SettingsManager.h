#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include "Settings.h"
#include <fstream>
#include <iostream>

class SettingsManager {
public:
    SettingsManager(const std::string& configFilePath = "settings.txt") : configFilePath(configFilePath) {
        loadSettings();
    }

    ~SettingsManager() {
        saveSettings();
    }

    void saveSettings() const {
        std::ofstream file(configFilePath);
        if (file) {
            file << "TemperatureUnit=" << settings.temperatureUnit << '\n';
            file << "WindSpeedUnit=" << settings.windSpeedUnit << '\n';
            file << "precipitationUnit=" << settings.precipitationUnit << '\n';
            file << "WindSpeedUnit=" << settings.windSpeedUnit << '\n';
            file << "timeFormat=" << settings.timeFormat << '\n';
            file << "pastDays=" << settings.pastDays << '\n';
            file << "forecastDays=" << settings.forecastDays << '\n';
            file << "startDate=" << settings.startDate << '\n';
            file << "endDate=" << settings.endDate << '\n';
            file << "timeZone=" << settings.timeZone << '\n';
        }
    }

    void loadSettings() {
        std::ifstream file(configFilePath);
        std::string line;
        while (getline(file, line)) {
            auto delimiterPos = line.find('=');
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            if (key == "TemperatureUnit") {
                settings.temperatureUnit = value;
            }
            else if (key == "WindSpeedUnit") {
                settings.windSpeedUnit = value;
            }
            // Add other settings as needed
        }
    }

    Settings& getSettings() {
        return settings;
    }

private:
    Settings settings;
    std::string configFilePath;
};

#endif // SETTINGS_MANAGER_H
