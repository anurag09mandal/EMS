// ============================================================================
// File: logcollector_application.hpp
// Description: Custom implementation derived from BaseApp
// Author:  Anurag
// Created: 2025-20-11
// Updated:
// License: Internal use only 
// ============================================================================

#pragma once

#include "base_app.hpp"
#include <thread>
#include <chrono>

// LogCollector class:
// Inherits from BaseApp and implements application-specific behavior.
// Handles incoming MQTT messages and heartbeat events.
class LogCollector : public BaseApp {
public:
    // Constructor that passes client ID to BaseApp
    explicit LogCollector(const std::string& client_id);
    ~LogCollector() override;
    void start() override;
    void cleanup() override;

protected:
    void handle_request(const std::string& tid, const std::string& timestamp, const std::string& source, const std::string& service, const nlohmann::json& message) override;
    void handle_response(const std::string& tid, const std::string& timestamp, const std::string& source, const std::string& service, const nlohmann::json& message) override;
    void handle_response_system_settings(const nlohmann::json& message);
    void handle_response_persistent_states(const nlohmann::json& message);
    void handle_notification(const std::string& timestamp, const std::string& source, const std::string& service, const nlohmann::json& message) override;
    void handle_noti_system_settings_changed(std::vector<std::string> keys) override;
    void handle_noti_persistent_states_changed(std::vector<std::string> keys) override;
    void handle_message(const std::string& topic, const nlohmann::json& message) override;

private:
    void message_sender_loop(std::stop_token stop_token);
    nlohmann::json generate_test_message();
    void test_set_configuration();
    void test_select_records();
    void handle_response_select_records(const nlohmann::json& message);
    void test_upsert_records();
    void handle_response_upsert_records(const nlohmann::json& message);
    
    std::jthread message_sender_thread_;
    bool iothub_connected_ = false;
};

