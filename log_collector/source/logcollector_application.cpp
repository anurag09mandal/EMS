// ============================================================================
// File: logcollector_application.cpp
// Description: Implementation of logcollector_application class handling custom
// logic Author: Created: 2025-11-20 Updated: License: Internal use only
// ============================================================================

#include "logcollector_application.hpp"
#include "edge_logger.hpp"
//#include "external_device.hpp"
#include "logger_utils.hpp"
#include "metric_loader.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <unistd.h>

// Constructor: Initializes the LogCollector with a given client ID
LogCollector::LogCollector(const std::string &client_id) : BaseApp(client_id) {
  EdgeLogger::get().set_app_name_mapping(client_id, "LogClctr");
  std::srand(std::time(nullptr));
}

LogCollector::~LogCollector() {}

void LogCollector::cleanup() {
  LOG(INFO) << "LogCollector cleanup" << std::endl;
  cleanup_called();

  if (message_sender_thread_.joinable()) {
    message_sender_thread_.request_stop();
  }
  DeviceManager::instance().cleanup();
}

void LogCollector::start() {
  notify_app_ready();
  LoggerUtils::instance().initLogMapping();//Input log
  MetricLoader::instance(REGISTER_MAP_PATH, CONFIG_MAP_PATH);
  MetricLoader::instance().initializeMetricLoading();
  DeviceManager::instance().initThreads();
  message_sender_thread_ =
      std::jthread(&LogCollector::message_sender_loop, this);
}

void LogCollector::message_sender_loop(std::stop_token stop_token) {
  std::cout << "Message sender loop started v1.0" << std::endl;

  auto requestMetrics = MetricLoader::instance().getRequestMetrics();
  while (!stop_token.stop_requested()) {
    for (const auto &req : requestMetrics) {//requestMetrics
      nlohmann::json payload;
      payload["metricPath"] = req.metricPath;
      publish_request(APPID_ENERGY_LINK, SERVICE_SEND_READ_METRIC,
                      payload); // TODO: Handle the return result
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
}

void LogCollector::handle_on_demand_read_response(
    const nlohmann::json &message) {
  if (message.contains("error") && message["error"].is_string() ) {
    std::cout << "[OnDemandResponse] Error in message:"
              << message["error"].get<std::string>() << std::endl;
    return;
  }
  
  if(message.contains("error_code") && message["error_code"].is_string())
  {
    const auto ec = message["error_code"].get<std::string>();
    if(ec != "NONE")
    {
      std::cout << "[On DemandResponse] Error code:" << ec << std::endl;
      return;
    }
  }
  if (!message.contains("payload") || !message["payload"].is_object()) {
    std::cerr << "[OnDemandResponse] Missing required field in the message"
              << std::endl;
    return;
  }
  const auto& payload = message["payload"];

  if (!payload.contains("metricPath") || !payload.contains("value")) {
    std::cerr << "[OnDemandResponse] Missing required field in the message"
              << std::endl;
    return;
  }

  std::string metricPath = payload["metricPath"].get<std::string>();
  std::string value ;//= message["value"].dump();
   
   if(payload["value"].is_string())
   {
    value = payload["value"].get<std::string>();
   }
   else if(payload["value"].is_number_integer())
   {
    value = std::to_string(payload["value"].get<long long>());
   }
   else if(payload["value"].is_number_float())
   {
    value = std::to_string(payload["value"].get<double>());
   }
   else if(payload["value"].is_number_unsigned())
   {
     value = std::to_string(payload["value"].get<unsigned long long>());
   }






  std::string metricName = metricPath.substr(metricPath.find_last_of('/') + 1);

  auto &logMapping = LoggerUtils::instance().getLogMapping();
  auto it = logMapping.find(metricName);
  if (it == logMapping.end()) {
    std::cerr << "[OnDemandResponse] Unknown metric: " << metricName
              << std::endl;
    return;
  }
  std::string category = it->second.logType; // e.g., BO
  std::string deviceType = DeviceManager::instance().getDeviceTypeForCategory(category);
  if (deviceType.empty()) {
    std::cerr << "[OnDemandResponse] Unknown category: " << category
              << std::endl;
    return;
  }
  std::cout << "Enque Task for External Device: " << deviceType << "of Log Type:" <<  category << std::endl;
  DeviceManager::instance().enqueueTask(deviceType,
                            {metricName, value, it->second.logFileName});
}

void LogCollector::handle_request(const std::string &tid,
                                  const std::string &timestamp,
                                  const std::string &source,
                                  const std::string &service,
                                  const nlohmann::json &message) {
  if (service == SERVICE_SHUTDOWN_APPLICATION ||
      service == SERVICE_SHUTDOWN_APPLICATION_FOR_SYSTEM_REBOOT) {
    std::cout << "Exit Application Log Cl" << std::endl;
    cleanup();
    nlohmann::json res_payload;
    publish_response(tid, res_payload);
  }
}

void LogCollector::handle_response(const std::string &tid,
                                   const std::string &timestamp,
                                   const std::string &source,
                                   const std::string &service,
                                   const nlohmann::json &message) {
  //LOG(DEBUG) << "[test] handle_response" << source << " "  << service << " :" << message.dump(4) << std::endl;
  if (service == SERVICE_SEND_READ_METRIC) // SERVICE_ON_DEMAND_READ)
  {
     std::cout << "Handle demand response" << std::endl;
     LOG(DEBUG) << "[test] handle_response" << source << " "  << service << " :" << message.dump(4) << std::endl;
     handle_on_demand_read_response(message);
  }
}

void LogCollector::handle_notification(const std::string &timestamp,
                                       const std::string &source,
                                       const std::string &service,
                                       const nlohmann::json &message) {
  //LOG(DEBUG) << "[test] handle_notification" << source << " "  << service << " :" << message.dump(4) << std::endl;
  if (service == NOTI_TELEMETRY) {
       //LOG(DEBUG) << "[test] handle_notification" << source << " "  << service << " :" << message.dump(4) << std::endl;
  }
  if (service == NOTI_CHANGED_DEVICE_CONNECTION) {
  }
}

void LogCollector::handle_noti_system_settings_changed(
    std::vector<std::string> keys) {}

void LogCollector::handle_noti_persistent_states_changed(
    std::vector<std::string> keys) {}

void LogCollector::handle_message(const std::string &topic,
                                  const nlohmann::json &message) {
  LOG(DEBUG) << "Callback for handling messages" << "topic : " << topic
             << message["message"].get<std::string>() << std::endl;
}
