
#include "metric_loader.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include "logger_utils.hpp"

#define REGISTER_MAP_PATH "/edge/sp/register_map.json"
#define CONFIG_MAP_PATH   "/edge/sp/configuration.json"
#define SPI_SERIAL_NUMBER "sn234234234"
#define CAN_SERIAL_NUMBER "sn123123123"

std::vector<MetricRequest> requestMetrics;
std::vector<MetricRequest> periodicMetrics;

void loadMetricPaths(const std::string& jsonFilePath, const std::string& defaultSerial,
                     const std::string& specialSerial, const std::string& specialMapId) {
    auto& logMapping = LoggerUtils::instance().getLogMapping();
    std::ifstream file(jsonFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open register map file: " << jsonFilePath << std::endl;
        return;
    }

    nlohmann::json data;
    file >> data;

    for (const auto& regMap : data["registerMaps"]) {
        std::string regMapId = regMap.value("id", "");
        std::string serialNumber = (regMapId == specialMapId) ? specialSerial : defaultSerial;

        for (const auto& regGroup : regMap["registerGroups"]) {
            std::string regGroupId = regGroup.value("id", "");
            std::string operation = regGroup.value("operation", "");
            int periodMs = regGroup.value("periodMs", 0);

            if (regGroupId == "write_group" || regGroupId == "jf2_commands_write" || regGroupId == "jf2_normal_write")
                continue;
            if (operation == "onDemandWrite")
                continue;

            for (const auto& reg : regGroup["registers"]) {
                std::string regId = reg.value("id", "");
                 
               
                for (const auto& metric : reg["metrics"]) {
                    std::string metricId = metric.value("id", "");
                    std::string path = serialNumber + "/" + regMapId + "/" + regGroupId + "/" + regId + "/" + metricId;
                    std::cerr << "request for Data Name: " << metricId << std::endl;
                    auto it = logMapping.find(metricId);
                    if (it == logMapping.end()) {
                         std::cerr << "[No log] request for Data Name in internal LUT: " << metricId << std::endl;
                         continue;
                    }
                    
                    if(serialNumber == specialSerial )
                    continue;
                    if (periodMs == 0) {
                        requestMetrics.push_back({path});
                    } else {
                        periodicMetrics.push_back({path});
                    }
                }
            }
        }
    }

    for(const auto& req : requestMetrics)
    {
        std::cout << req.metricPath << std::endl;
    }
}

void initializeMetricLoading() {
    std::ifstream configFile(CONFIG_MAP_PATH);
    if (!configFile.is_open()) {
        std::cerr << "Failed to open configuration file: " << CONFIG_MAP_PATH << std::endl;
        return;
    }

    nlohmann::json configJson;
    configFile >> configJson;

    std::string SPISerial = "sn_default";
    std::string CANSerial = "sn_default";

    if (configJson.contains("deviceList") && configJson["deviceList"].is_array()) {
        auto deviceList = configJson["deviceList"];
        if (deviceList.size() > 0 && deviceList[0].contains("serialNum")) {
            SPISerial = deviceList[0]["serialNum"].get<std::string>();
        }
        if (deviceList.size() > 1 && deviceList[1].contains("serialNum")) {
            CANSerial = deviceList[1]["serialNum"].get<std::string>();
        }
    }

    loadMetricPaths(REGISTER_MAP_PATH, SPISerial, CANSerial, "pcsx_can_reg_map_4pcs");
}
