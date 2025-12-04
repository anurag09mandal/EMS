
// metric_loader.hpp
#ifndef METRIC_LOADER_HPP
#define METRIC_LOADER_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include "logger_utils.hpp"

#define REGISTER_MAP_PATH "/edge/sp/register_map.json"
#define CONFIG_MAP_PATH "/edge/sp/configuration.json"

struct MetricRequest {
    std::string metricPath;
};

class MetricLoader {
public:
    
    static MetricLoader& instance(const std::string& registerMapPath = std::string(),
                                const std::string&  configMapPath = std::string())
    {
        static MetricLoader instanceobj(registerMapPath,configMapPath);    
        return instanceobj;
    }

    void initializeMetricLoading();
    const std::vector<MetricRequest>& getRequestMetrics() const;
    const std::vector<MetricRequest>& getPeriodicMetrics() const;
    void loadMetricPaths(const std::string& jsonFilePath,
                        const std::string& defaultSerial,
                        const std::string& specialSerial,
                        const std::string& specialMapId);

private:
    MetricLoader(std::string registerMapPath, std::string configMapPath)
       : registerMapPath_(std::move(registerMapPath)),
       configMapPath_(std::move(configMapPath))
    {

    }
    std::string registerMapPath_;
    std::string configMapPath_;
    std::vector<MetricRequest> requestMetrics_;
    std::vector<MetricRequest> periodicMetrics_;
};



#endif