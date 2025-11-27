
#ifndef METRIC_LOADER_HPP
#define METRIC_LOADER_HPP

#include <string>
#include <vector>

struct MetricRequest {
    std::string metricPath;
};

extern std::vector<MetricRequest> requestMetrics;
extern std::vector<MetricRequest> periodicMetrics;

void initializeMetricLoading();
void loadMetricPaths(const std::string& jsonFilePath, const std::string& defaultSerial,
                     const std::string& specialSerial, const std::string& specialMapId);

#endif
