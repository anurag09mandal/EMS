
#include "edge_logger.hpp"
#include "logger_utils.hpp"
#include "metric_loader.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <unistd.h>
#include <queue>
#include <condition_variable>
#include <thread>

struct LogTask {
  std::string metricName;
  std::string value;
  std::string fileName;
};


class DeviceManager {
public:
    static DeviceManager& instance()
    {
        static DeviceManager instanceobj;    
        return instanceobj;
    }



  std::string getDeviceTypeForCategory(const std::string &category) const;

  void initThreads();
  void enqueueTask(const std::string &deviceType, LogTask task);
  void cleanup();

private:
  DeviceManager() = default;
  void deviceWorker(std::stop_token stoken, const std::string &deviceType);

  std::unordered_map<std::string, std::queue<LogTask>> deviceQueues;
  std::unordered_map<std::string, std::mutex> deviceMutexes;
  std::unordered_map<std::string, std::condition_variable> deviceCVs;
  std::unordered_map<std::string, std::jthread> deviceThreads;
  // const std::unordered_map<std::string, std::vector<std::string>>
  // &deviceCategoryMap;

  const std::unordered_map<std::string, std::vector<std::string>>
      deviceCategoryMap = {{"BMS", {"BO", "BF", "BM", "BLC"}},
                           {"PCS", {"PCCM", "PCF", "PCFM"}},
                           {"PMU", {"PMM", "PMF"}},
                           {"MI", {"ME", "MM", "MDI"}},
                           {"EMS", {"EM", "EI"}},
                           {"AHM", {"AHM"}}};
};
