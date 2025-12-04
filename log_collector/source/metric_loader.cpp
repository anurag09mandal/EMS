
#include "metric_loader.hpp"
#include <fstream>
#include <iostream>

#define SPI_SERIAL_NUMBER "sn234234234"
#define CAN_SERIAL_NUMBER "sn123123123"

// MetricLoader::MetricLoader(const std::string& registerMapPath, const std::string& configMapPath)
//     : registerMapPath_(registerMapPath), configMapPath_(configMapPath) {}

void MetricLoader::initializeMetricLoading() {
    std::ifstream configFile(configMapPath_);
    if (!configFile.is_open()) {
        std::cerr << "Failed to open configuration file: " << configMapPath_ << std::endl;
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

    loadMetricPaths(registerMapPath_, SPISerial, CANSerial, "pcsx_can_reg_map_4pcs");
}

void MetricLoader::loadMetricPaths(const std::string& jsonFilePath,
                                   const std::string& defaultSerial,
                                   const std::string& specialSerial,
                                   const std::string& specialMapId) {
    auto& logMapping = LoggerUtils::instance().getLogMapping();
    std::ifstream file(jsonFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open register map file: " << jsonFilePath << std::endl;
        return;
    }

    nlohmann::json data;
    file >> data;
    std::unordered_set<std::string> jsonMetrics;

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
                    jsonMetrics.insert(metricId);

                    auto it = logMapping.find(metricId);
                    if (it == logMapping.end()) {
                        continue;
                    }

                    std::string path = serialNumber + "/" + regMapId + "/" + regGroupId +
                                       "/" + regId + "/" + metricId;

                    if (periodMs == 0) {
                        requestMetrics_.push_back({path});
                    } else {
                        periodicMetrics_.push_back({path});
                    }
                }
            }
        }
    }
    auto requestMetrics = MetricLoader::instance().getRequestMetrics();

    for(const auto& req: requestMetrics)
    {
        std::cout << req.metricPath << std::endl;
    }

    std::cout << "\n=== Missing Metrics in JSON ===\n";
    for (const auto& [metricId, info] : logMapping) {
        if (jsonMetrics.find(metricId) == jsonMetrics.end()) {
            std::cerr << "[Warning] Metric in logMapping but missing in JSON: "
                      << metricId << std::endl;
        }
    }
}

const std::vector<MetricRequest>& MetricLoader::getRequestMetrics() const {
    return requestMetrics_;
}

const std::vector<MetricRequest>& MetricLoader::getPeriodicMetrics() const {
    return periodicMetrics_;
}
