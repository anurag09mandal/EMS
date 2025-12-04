#include "external_device.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include "edge_logger.hpp"
#include "logger_utils.hpp"
#include "metric_loader.hpp"
#include <unistd.h>


 void DeviceManager:: initThreads() {
        for (const auto& [deviceType, categories] : deviceCategoryMap) {
            deviceThreads[deviceType] = std::jthread(&DeviceManager::deviceWorker, this, deviceType);
        }
}

std::string DeviceManager::getDeviceTypeForCategory(const std::string& category) const {
    for (const auto& [deviceType, categories] : deviceCategoryMap) {
        if (std::find(categories.begin(), categories.end(), category) != categories.end()) {
            return deviceType;
        }
    }
    return {};
}


void DeviceManager::enqueueTask(const std::string& deviceType, LogTask task) {
    {
        std::lock_guard<std::mutex> lock(deviceMutexes[deviceType]);
        deviceQueues[deviceType].push(std::move(task));
    }
    deviceCVs[deviceType].notify_one();
}

 void DeviceManager:: cleanup() {
        for (auto& [deviceType, thread] : deviceThreads) {
            if (thread.joinable()) thread.request_stop();
            deviceCVs[deviceType].notify_all();
        }
        for (auto& [deviceType, thread] : deviceThreads) {
            if (thread.joinable()) thread.join();
        }
        deviceThreads.clear();
}

void DeviceManager::deviceWorker(std::stop_token stoken, const std::string& deviceType) {
    while (!stoken.stop_requested()) {
        std::unique_lock<std::mutex> lock(deviceMutexes[deviceType]);
        deviceCVs[deviceType].wait(lock, [&] {
            return !deviceQueues[deviceType].empty() || stoken.stop_requested();
        });

        if (stoken.stop_requested()) break;

        auto task = deviceQueues[deviceType].front();
        deviceQueues[deviceType].pop();
        lock.unlock();

        LoggerUtils::instance().logMetricToCSV(task.metricName, task.value, task.fileName);
    }
}