#include <iostream>
#include <vector>
#include "LocationManager.h"
#include "ApiHandler.h"
#include "SettingsManager.h"

#undef max


int main() {
    LocationManager locManager("locations.txt");
    SettingsManager settingsManager;
    ApiHandler apiHandler(settingsManager);
    int choice;

    do {
        std::cout << "Menu:\n"
            << "1. Weather Forecast\n"
            << "2. Add location\n"
            << "3. Remove location\n"
            << "4. Modify location data\n"
            << "5. Search for locations\n"
            << "6. Set favourite locations\n"
            << "7. Read Data of locations\n"
            << "8. Settings\n"
            << "9. Exit\n"
            << "Enter your choice: ";
        std::cin >> choice;

        std::string locationId, locationName, startDate, endDate;
        double latitude, longitude;


        switch (choice) {
        case 1:
        { // Main Hourly Weather
            std::cout << "Enter latitude and longitude: ";
            std::cin >> latitude >> longitude;
            std::cout << "Is this request for historical data (yes/no)? ";
            std::string historical;
            std::cin >> historical;
            std::vector<std::string> parameters = { "temperature_2m", "relative_humidity_2m", "precipitation", "surface_pressure", "cloud_cover", "wind_gusts_10m" };
            if (historical == "yes") {
                std::cout << "Enter start date (YYYY-MM-DD): ";
                std::cin >> startDate;
                std::cout << "Enter end date (YYYY-MM-DD): ";
                std::cin >> endDate;
            }
            try {
                auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, parameters, startDate, endDate);
                if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                    std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                }
                else {
                    std::cout << "Weather data: " << jsonResponse.dump(4) << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Exception occurred: " << e.what() << '\n';
            }
            break;
        }
        case 2: { // Add location
            std::cout << "Enter location ID (alphanumeric), Name, Latitude, and Longitude:\n";
            while (true) {
                std::cin >> locationId >> locationName >> latitude >> longitude;
                if (!std::cin.fail() && !locationId.empty() && !locationName.empty()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    LocationInfo location{ locationId, locationName, latitude, longitude };
                    locManager.addLocation(location);
                    std::cout << "Location added successfully!\n";
                    break;
                }
                else {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "Invalid input. Please enter valid ID, name, latitude, and longitude:\n";
                }
            }
            break;
        }
        case 3: { // Remove location
            std::cout << "Enter location ID to remove: ";
            std::cin >> locationId;
            if (!locationId.empty()) {
                locManager.removeLocationById(locationId);
                std::cout << "Location removed successfully!\n";
            }
            else {
                std::cerr << "Invalid input. Location ID cannot be empty.\n";
            }
            break;
        }
        case 4: { // Modify location data
            std::cout << "Enter location ID to modify: ";
            std::cin >> locationId;
            std::cout << "Enter new Name, Latitude, and Longitude:\n";
            while (true) {
                std::cin >> locationName >> latitude >> longitude;
                if (!std::cin.fail() && !locationName.empty()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    LocationInfo newLocation{ locationId, locationName, latitude, longitude };
                    locManager.modifyLocation(locationId, newLocation);
                    std::cout << "Location data modified successfully!\n";
                    break;
                }
                else {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "Invalid input. Please enter a valid name, latitude, and longitude:\n";
                }
            }
            break;
        }
        case 5: { // Search for locations
            std::cout << "Enter search term: ";
            std::cin >> locationName;
            if (!locationName.empty()) {
                auto foundLocations = locManager.searchLocations(locationName);
                for (const auto& loc : foundLocations) {
                    locManager.displayLocation(loc);
                }
                if (foundLocations.empty()) {
                    std::cout << "No locations found.\n";
                }
            }
            else {
                std::cerr << "Invalid input. Search term cannot be empty.\n";
            }
            break;
        }
        case 6: { // Manage favorite locations
            std::cout << "Favorite Locations:\n"
                << "1. Add to favorites\n"
                << "2. List favorites\n"
                << "Enter your choice: ";
            int favChoice;
            std::cin >> favChoice;
            switch (favChoice) {
            case 1: {
                std::cout << "Enter location ID to add to favorites: ";
                std::cin >> locationId;
                locManager.addFavorite(locationId);
                break;
            }
            case 2:
                locManager.listFavorites();
                break;
            default:
                std::cout << "Invalid choice for favorites menu.\n";
            }
            break;

        }
        case 7: { // Nested menu for different weather data readings
            std::cout << "Select the type of weather data to read:\n"
                << "1. Main Hourly Weather\n"
                << "2. Solar Radiation\n"
                << "3. Pressure\n"
                << "4. Weather Models\n"
                << "5. Daily Weather Forecast\n"
                << "6. Historical Data\n"
                << "7. Additional Weather\n"
                << "8. Additional Hourly Weather\n"
                << "Enter your choice: ";
            int subChoice;
            std::cin >> subChoice;
            switch (subChoice) {
            case 1: { // Main Hourly Weather
                std::cout << "Enter latitude and longitude: ";
                std::cin >> latitude >> longitude;
                try {
                    std::vector<std::string> params = { "temperature_2m", "relative_humidity_2m", "precipitation", "surface_pressure", "cloud_cover", "wind_gusts_10m" };
                    auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params);
                    if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                        std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                    }
                    else {
                        std::cout << "Hourly Weather Data: " << jsonResponse.dump(4) << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception occurred: " << e.what() << '\n';
                }
                break;
            }
            case 2: { // Solar Radiation
                std::cout << "Enter latitude and longitude for solar radiation data: ";
                std::cin >> latitude >> longitude;
                std::vector<std::string> params = {
                    "temperature_2m", "shortwave_radiation", "direct_radiation", "diffuse_radiation", "direct_normal_irradiance", "global_tilted_irradiance", "terrestrial_radiation",
                    "shortwave_radiation_instant", "direct_radiation_instant", "diffuse_radiation_instant", "direct_normal_irradiance_instant", "global_tilted_irradiance_instant", "terrestrial_radiation_instant"
                };
                try {
                    auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params);
                    if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                        std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                    }
                    else {
                        std::cout << "Solar Radiation Data: " << jsonResponse.dump(4) << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception occurred: " << e.what() << '\n';
                }
                break;
            }
            case 3: { // Nested menu for different weather data readings
                std::cout << "Select the type of pressure data to read:\n"
                    << "1. Temperature\n"
                    << "2. Relative Humidity\n"
                    << "3. Cloud Cover\n"
                    << "4. Wind Speed\n"
                    << "5. Wind Direction\n"
                    << "6. Geopotential Height\n"
                    << "Enter your choice: ";
                int subChoice;
                std::cin >> subChoice;
                switch (subChoice) {
                case 1: {
                    std::cout << "Enter latitude and longitude: ";
                    std::cin >> latitude >> longitude;
                    try {
                        std::vector<std::string> params = {
                            "temperature_1000hPa", "temperature_975hPa", "temperature_950hPa", "temperature_925hPa", "temperature_900hPa",
                            "temperature_850hPa", "temperature_800hPa", "temperature_700hPa", "temperature_600hPa", "temperature_500hPa",
                            "temperature_400hPa", "temperature_300hPa", "temperature_250hPa", "temperature_200hPa", "temperature_150hPa",
                            "temperature_100hPa", "temperature_70hPa", "temperature_50hPa", "temperature_30hPa"
                        };
                        auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params);
                        if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                            std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                        }
                        else {
                            std::cout << "Temperature Data: " << jsonResponse.dump(4) << std::endl;
                        }
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Exception occurred: " << e.what() << '\n';
                    }
                    break;
                }
                case 2: {
                    std::cout << "Enter latitude and longitude: ";
                    std::cin >> latitude >> longitude;
                    std::vector<std::string> params = {
                        "relative_humidity_1000hPa", "relative_humidity_975hPa", "relative_humidity_950hPa", "relative_humidity_925hPa", "relative_humidity_900hPa",
                        "relative_humidity_850hPa", "relative_humidity_800hPa", "relative_humidity_700hPa", "relative_humidity_600hPa", "relative_humidity_500hPa",
                        "relative_humidity_400hPa", "relative_humidity_300hPa", "relative_humidity_250hPa", "relative_humidity_200hPa", "relative_humidity_150hPa",
                        "relative_humidity_100hPa", "relative_humidity_70hPa", "relative_humidity_50hPa", "relative_humidity_30hPa"
                    };
                    try {
                        auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params);
                        if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                            std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                        }
                        else {
                            std::cout << "Relative Humidity Data: " << jsonResponse.dump(4) << std::endl;
                        }
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Exception occurred: " << e.what() << '\n';
                    }
                    break;
                }
                case 3: {
                    std::cout << "Enter latitude and longitude: ";
                    std::cin >> latitude >> longitude;
                    std::vector<std::string> params = {
                        "cloud_cover_1000hPa", "cloud_cover_975hPa", "cloud_cover_950hPa", "cloud_cover_925hPa", "cloud_cover_900hPa",
                        "cloud_cover_850hPa", "cloud_cover_800hPa", "cloud_cover_700hPa", "cloud_cover_600hPa", "cloud_cover_500hPa",
                        "cloud_cover_400hPa", "cloud_cover_300hPa", "cloud_cover_250hPa", "cloud_cover_200hPa", "cloud_cover_150hPa",
                        "cloud_cover_100hPa", "cloud_cover_70hPa", "cloud_cover_50hPa", "cloud_cover_30hPa"
                    };
                    try {
                        auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params);
                        if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                            std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                        }
                        else {
                            std::cout << "Cloud Cover Data: " << jsonResponse.dump(4) << std::endl;
                        }
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Exception occurred: " << e.what() << '\n';
                    }
                    break;
                }
                case 4: {
                    std::cout << "Enter latitude and longitude: ";
                    std::cin >> latitude >> longitude;
                    std::vector<std::string> params = {
                        "windspeed_1000hPa", "windspeed_975hPa", "windspeed_950hPa", "windspeed_925hPa", "windspeed_900hPa",
                        "windspeed_850hPa", "windspeed_800hPa", "windspeed_700hPa", "windspeed_600hPa", "windspeed_500hPa",
                        "windspeed_400hPa", "windspeed_300hPa", "windspeed_250hPa", "windspeed_200hPa", "windspeed_150hPa",
                        "windspeed_100hPa", "windspeed_70hPa", "windspeed_50hPa", "windspeed_30hPa"
                    };
                    try {
                        auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params);
                        if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                            std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                        }
                        else {
                            std::cout << "Wind Speed Data: " << jsonResponse.dump(4) << std::endl;
                        }
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Exception occurred: " << e.what() << '\n';
                    }
                    break;
                }
                case 5: {
                    std::cout << "Enter latitude and longitude: ";
                    std::cin >> latitude >> longitude;
                    std::vector<std::string> params = {
                        "winddirection_1000hPa", "winddirection_975hPa", "winddirection_950hPa", "winddirection_925hPa", "winddirection_900hPa",
                        "winddirection_850hPa", "winddirection_800hPa", "winddirection_700hPa", "winddirection_600hPa", "winddirection_500hPa",
                        "winddirection_400hPa", "winddirection_300hPa", "winddirection_250hPa", "winddirection_200hPa", "winddirection 150hPa",
                        "winddirection_100hPa", "winddirection_70hPa", "winddirection_50hPa", "winddirection_30hPa"
                    };
                    try {
                        auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params);
                        if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                            std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                        }
                        else {
                            std::cout << "Wind Direction Data: " << jsonResponse.dump(4) << std::endl;
                        }
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Exception occurred: " << e.what() << '\n';
                    }
                    break;
                }
                case 6: {
                    std::cout << "Enter latitude and longitude: ";
                    std::cin >> latitude >> longitude;
                    std::vector<std::string> params = {
                        "geopotential_height_1000hPa", "geopotential_height_975hPa", "geopotential_height_950hPa", "geopotential_height_925hPa", "geopotential_height_900hPa",
                        "geopotential_height_850hPa", "geopotential_height_800hPa", "geopotential_height_700hPa", "geopotential_height_600hPa", "geopotential_height_500hPa",
                        "geopotential_height_400hPa", "geopotential_height_300hPa", "geopotential_height_250hPa", "geopotential_height_200hPa", "geopotential_height_150hPa",
                        "geopotential_height_100hPa", "geopotential_height_70hPa", "geopotential_height_50hPa", "geopotential_height_30hPa"
                    };
                    try {
                        auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params);
                        if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                            std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                        }
                        else {
                            std::cout << "Geopotential Height Data: " << jsonResponse.dump(4) << std::endl;
                        }
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Exception occurred: " << e.what() << '\n';
                    }
                    break;
                }
                default:
                    std::cout << "Invalid choice. Please enter a number between 1 and 8.\n";
                }
                break;
            }
            case 4: // Weather Models
                std::cout << "Functionality to be implemented...\n";
                break;
            case 5: // Daily Weather Forecast
            {
                std::cout << "Enter latitude and longitude: ";
                std::cin >> latitude >> longitude;
                std::vector<std::string> params = {
                    "weather_code", "temperature_2m_max", "temperature_2m_min", "apparent_temperature_max", "apparent_temperature_min",
                    "sunrise", "sunset", "daylight_duration", "sunshine_duration", "uv_index_max", "uv_index_clear_sky_max", "precipitation_sum",
                    "rain_sum", "showers_sum", "snowfall_sum", "precipitation_hours", "precipitation_probability_max", "wind_speed_10m_max",
                    "wind_gusts_10m_max", "wind_direction_10m_dominant", "shortwave_radiation_sum", "et0_fao_evapotranspiration"
                };
                try {
                    auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params);
                    if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                        std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                    }
                    else {
                        std::cout << "Daily Weather Forecast: " << jsonResponse.dump(4) << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception occurred: " << e.what() << '\n';
                }
                break;
            }
            case 6: // Historical Data
            {
                std::cout << "Enter latitude and longitude: ";
                std::cin >> latitude >> longitude;
                std::cout << "Enter start date (YYYY-MM-DD): ";
                std::cin >> startDate;
                std::cout << "Enter end date (YYYY-MM-DD): ";
                std::cin >> endDate;
                
                std::vector<std::string> params = {
                    "temperature_2m", "relative_humidity_2m", "dew_point_2m", "apparent_temperature", "precipitation_probability",
                    "precipitation", "rain", "showers", "snowfall", "snow_depth", "weather_code", "pressure_msl", "surface_pressure",
                    "cloud_cover", "cloud_cover_low", "cloud_cover_mid", "cloud_cover_high", "visibility", "evapotranspiration",
                    "et0_fao_evapotranspiration", "vapour_pressure_deficit", "wind_speed_10m", "wind_speed_80m", "wind_speed_120m",
                    "wind_speed_180m", "wind_direction_10m", "wind_direction_80m", "wind_direction_120m", "wind_direction_180m",
                    "wind_gusts_10m", "temperature_80m", "temperature_120m", "temperature_180m", "soil_temperature_0cm",
                    "soil_temperature_6cm", "soil_temperature_18cm", "soil_temperature 54cm", "soil_moisture_0_to_1cm",
                    "soil_moisture_1_to_3cm", "soil_moisture_3_to_9cm", "soil_moisture_9_to_27cm", "soil_moisture_27_to 81cm"
                };
                try {
                    auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params, startDate, endDate);
                    if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                        std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                    }
                    else {
                        std::cout << "Historical Weather Data: " << jsonResponse.dump(4) << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception occurred: " << e.what() << '\n';
                }
                break;
            }
            case 7: // Additional Weather
            {
                std::cout << "Enter latitude and longitude: ";
                std::cin >> latitude >> longitude;
                std::vector<std::string> params = {
                    "uv_index", "uv_index_clear_sky", "is_day", "cape", "freezing_level_height", "sunshine_duration"
                };
                try {
                    auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params);
                    if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                        std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                    }
                    else {
                        std::cout << "Additional Weather: " << jsonResponse.dump(4) << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception occurred: " << e.what() << '\n';
                }
                break;
            }
            case 8: // Additional Hourly Weather
            {
                std::cout << "Enter latitude and longitude: ";
                std::cin >> latitude >> longitude;
                std::vector<std::string> params = {
                    "temperature_2m", "relative_humidity_2m", "dew_point_2m", "apparent_temperature", "precipitation_probability",
                    "precipitation", "rain", "showers", "snowfall", "snow_depth", "weather_code", "pressure_msl", "surface_pressure",
                    "cloud_cover", "cloud_cover_low", "cloud_cover_mid", "cloud_cover_high", "visibility", "evapotranspiration",
                    "et0_fao_evapotranspiration", "vapour_pressure_deficit", "wind_speed_10m", "wind_speed_80m", "wind_speed_120m",
                    "wind_speed 180m", "wind_direction_10m", "wind_direction_80m", "wind_direction 120m", "wind_direction 180m",
                    "wind_gusts_10m", "temperature_80m", "temperature_120m", "temperature_180m", "soil_temperature_0cm",
                    "soil_temperature_6cm", "soil_temperature_18cm", "soil_temperature 54cm", "soil_moisture_0_to_1cm",
                    "soil_moisture_1_to_3cm", "soil_moisture_3_to 9cm", "soil_moisture_9_to 27cm", "soil_moisture_27_to_81cm"
                };
                try {
                    auto jsonResponse = apiHandler.fetchWeatherData(latitude, longitude, params);
                    if (jsonResponse.contains("error") && jsonResponse["error"] == true) {
                        std::cout << "Error fetching data: " << jsonResponse["reason"] << std::endl;
                    }
                    else {
                        std::cout << "Additional Hourly Weather: " << jsonResponse.dump(4) << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Exception occurred: " << e.what() << '\n';
                }
                break;
            }
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 8.\n";
            }
            break;
        }
        case 8: { // Settings management
            std::cout << "Settings Menu:\n"
                << "1. Change Temperature Unit\n"
                << "2. Change Wind Speed Unit\n"
                << "3. Change precipitation Units\n"
                << "4. Change Time Format\n"
                << "5. Change Past Days\n"
                << "6. Change Number of Forecast Days"
                << "7. Change Start Date\n"
                << "8. Change End Date\n"
                << "9. Change Time Zone\n"
                << "10. Save and Exit Settings\n"
                << "Enter your choice: ";
            int settingsChoice;
            std::cin >> settingsChoice;
            switch (settingsChoice) {
            case 1:
                std::cout << "Enter new Temperature Unit (Celsius/Fahrenheit): ";
                std::cin >> settingsManager.getSettings().temperatureUnit;
                break;
            case 2:
                std::cout << "Enter new Wind Speed Unit (km/h, m/s): ";
                std::cin >> settingsManager.getSettings().windSpeedUnit;
                break;
            case 3:
                std::cout << "Enter new Percipitation Unit (mm, in): ";
                std::cin >> settingsManager.getSettings().precipitationUnit;
                break;
            case 4:
                std::cout << "Enter new Time Format (ISO, UNIX): ";
                std::cin >> settingsManager.getSettings().timeFormat;
                break;
            case 5:
                std::cout << "Enter new Past Days (int): ";
                std::cin >> settingsManager.getSettings().pastDays;
                break;
            case 6:
                std::cout << "Enter new Number of Forecast Days (int): ";
                std::cin >> settingsManager.getSettings().forecastDays;
                break;
            case 7:
                std::cout << "Enter new Start Date: ";
                std::cin >> settingsManager.getSettings().startDate;
                break;
            case 8:
                std::cout << "Enter new End Date: ";
                std::cin >> settingsManager.getSettings().endDate;
                break;
            case 9:
                std::cout << "Enter new Time Zone: ";
                std::cin >> settingsManager.getSettings().timeZone;
                break;
            case 10:
                settingsManager.saveSettings();
                std::cout << "Settings saved!\n";
                break;
            default:
                std::cout << "Invalid option. Try again.\n";
                break;
            }
            break;
        }
        
        case 9: // Exit
            std::cout << "Exiting program.\n";
            break;
        default:
            std::cerr << "Invalid choice. Please enter a number between 1 and 6.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (choice != 9);

    return 0;


}
