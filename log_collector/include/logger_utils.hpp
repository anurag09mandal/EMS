
#pragma once
#include <unordered_map>
#include <string>

struct LogConfig {
    std::string regId;
    std::string logType;
    std::string logFileName;
    int writeCycleSeconds;
    int rotationSeconds;
    size_t fileSizeBytes;
};

class LoggerUtils {
public:
    static LoggerUtils& instance() {
        static LoggerUtils instance;
        return instance;
    }

    std::unordered_map<std::string, LogConfig>& getLogMapping() {
        return logMapping_;
    }

    void initLogMapping();
    bool shouldLog(const std::string& metricName); 
    void checkAndRotateFile(const std::string& fileName); 
    void logMetricToCSV(const std::string& metricName,const std::string& value,const std::string& fileName);
private:
    LoggerUtils() = default;
    std::unordered_map<std::string, LogConfig> logMapping_;
};
