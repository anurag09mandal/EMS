
#include "logger_utils.hpp"
#include <unordered_map>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <filesystem> // Add this for directory handling

#include <mutex>

static std::mutex logMutex;


#define LOG_DIR "/edge/log/device_logger/"
#define UPLOAD_DIR "/edge/log/toupload/device_logger/"
#define UPLOADED_DIR "/edge/log/uploaded/system_log/"

struct MetricState {
    std::chrono::system_clock::time_point lastLogged;
};

struct FileState {
    std::chrono::system_clock::time_point creationTime;
};

//static std::unordered_map<std::string, LogConfig> logMapping;
static std::unordered_map<std::string, MetricState> metricStates;
static std::unordered_map<std::string, FileState> fileStates;

void LoggerUtils::initLogMapping() {
    auto&logMapping = logMapping_;
    // logMapping["Group1_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group2_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group3_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group4_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group5_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group6_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group7_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group8_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group9_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group10_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group11_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group12_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group13_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group14_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group15_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group16_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group17_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group18_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["Group19_Status"] = LogConfig{"INSTALL_DATA_STATUS", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    // logMapping["SPI_Map_Version"] = LogConfig{"MCU_Info", "EM", "EMSP_Maintenance.csv", 900, 86400, 8192};
    // logMapping["ST8500_PE_FW_Version"] = LogConfig{"MCU_Info", "EM", "EMSP_Maintenance.csv", 900, 86400, 8192};
    // logMapping["ST8500_RTE_FW_Version"] = LogConfig{"MCU_Info", "EM", "EMSP_Maintenance.csv", 900, 86400, 8192};
    // logMapping["MCU_Flash_All_Checksum"] = LogConfig{"MCU_Info", "EM", "EMSP_Maintenance.csv", 900, 86400, 8192};
    logMapping["MCU_Boot_FW_VERSION"] = LogConfig{"MCU_Info", "EM", "EMSP_Maintenance.csv", 900, 86400, 8192};
    logMapping["MCU_FW_VERSION"] = LogConfig{"MCU_Info", "EM", "EMSP_Maintenance.csv", 900, 86400, 8192};
    logMapping["MCU_Startup_Status"] = LogConfig{"MCU_Status", "EM", "EMSP_Maintenance.csv", 900, 86400, 8192};
    logMapping["ConDevice_Install_SeqStatus"] = LogConfig{"MCU_Status", "EI", "EMSP_Installation.csv", 900, 86400, 8192};
    logMapping["MCU_Operation_Time"] = LogConfig{"MCU_Status", "EM", "EMSP_Maintenance.csv", 900, 86400, 8192};
    // logMapping["Init_Status"] = LogConfig{"EMS_Init_Status", "EM", "EMSP_Maintenance.csv", 900, 86400, 8192};
    // logMapping["Validation_Status"] = LogConfig{"PCS1_Validation_Status", "PCCM", "PCS_CAN_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Validation_Status"] = LogConfig{"PCS2_Validation_Status", "PCCM", "PCS_CAN_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Validation_Status"] = LogConfig{"PCS3_Validation_Status", "PCCM", "PCS_CAN_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Validation_Status"] = LogConfig{"PCS4_Validation_Status", "PCCM", "PCS_CAN_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Validation_Status"] = LogConfig{"AHUB_Validation_Status", "AHM", "Advanced_HUB_Monitoring.csv", 900, 86400, 8192};
    logMapping["Tot_Load_Active_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Tot_ESS_Apparent_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Tot_ESS_Active_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Tot_ESS_Reactive_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Tot_ESS_Power_Factor"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["MI_Apparent_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["MI_Active_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["MI_Reactive_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["MI_Power_Factor"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Ext_PV_Apparent_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Ext_PV_Active_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Ext_PV_Reactive_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Ext_PV_Power_Factor"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Grid_L1_Apparent_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_L1_Active_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_L1_Reactive_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Grid_L1_Power_Factor"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Grid_L2_Apparent_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_L2_Active_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_L2_Reactive_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Grid_L2_Power_Factor"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Grid_Apparent_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_Active_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_Reactive_Power"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Grid_Power_Factor"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_Freq"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["AC_Bus_Freq"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_L1N_V"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_L2N_V"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_LL_V"] = LogConfig{"PMU_Monitoring_Data_01", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["AC_Bus_L1N_V"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["AC_Bus_L2N_V"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["AC_Bus_LL_V"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Grid_L1_I"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["Grid_L2_I"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["MI_LL_V"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    // logMapping["MI_L1_I"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_Fault"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["Grid_Alarm"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["System_Fault"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["System_Warning"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["System_Alarm"] = LogConfig{"PMU_Monitoring_Data_02", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
  
    // logMapping["MI_Warning"] = LogConfig{"PMU_Monitoring_Data_03", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["GEM_Warning"] = LogConfig{"PMU_Monitoring_Data_03", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["DER1_Alarm"] = LogConfig{"PMU_Monitoring_Data_03", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["DER2_Alarm"] = LogConfig{"PMU_Monitoring_Data_03", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["DER3_Alarm"] = LogConfig{"PMU_Monitoring_Data_03", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["DER4_Alarm"] = LogConfig{"PMU_Monitoring_Data_03", "PMM", "PMU_Monitoring.csv", 900, 86400, 8192};
    logMapping["mlpe_mi_total_count"] = LogConfig{"MLPE_MI_Total_Count", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_version"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["serial_number"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_profile_crc"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["inverter_state"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_3"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_1"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_2"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_min"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_hour"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var_min"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["iac_rms"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vac_rms"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_frequency"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vpv"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["ipv"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["temperature"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_sub_version"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["lqi"] = LogConfig{"MI000_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["last_control_sec"] = LogConfig{"MI000_Monitoring_Data_1", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_version"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["serial_number"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_profile_crc"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["inverter_state"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_3"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_1"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_2"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_min"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_hour"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var_min"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["iac_rms"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vac_rms"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_frequency"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vpv"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["ipv"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["temperature"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_sub_version"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["lqi"] = LogConfig{"MI001_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["last_control_sec"] = LogConfig{"MI001_Monitoring_Data_1", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_version"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["serial_number"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_profile_crc"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["inverter_state"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_3"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_1"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_2"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_min"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_hour"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var_min"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["iac_rms"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vac_rms"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_frequency"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vpv"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["ipv"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["temperature"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_sub_version"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["lqi"] = LogConfig{"MI002_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["last_control_sec"] = LogConfig{"MI002_Monitoring_Data_1", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_version"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["serial_number"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_profile_crc"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["inverter_state"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_3"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_1"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_2"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_min"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_hour"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var_min"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["iac_rms"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vac_rms"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_frequency"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vpv"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["ipv"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["temperature"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_sub_version"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["lqi"] = LogConfig{"MI003_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["last_control_sec"] = LogConfig{"MI003_Monitoring_Data_1", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_version"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["serial_number"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_profile_crc"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["inverter_state"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_3"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_1"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_2"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_min"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_hour"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var_min"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["iac_rms"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vac_rms"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_frequency"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vpv"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["ipv"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["temperature"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_sub_version"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["lqi"] = LogConfig{"MI004_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["last_control_sec"] = LogConfig{"MI004_Monitoring_Data_1", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_version"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["serial_number"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_profile_crc"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["inverter_state"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_3"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_1"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_2"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_min"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_hour"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var_min"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["iac_rms"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vac_rms"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_frequency"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vpv"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["ipv"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["temperature"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_sub_version"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["lqi"] = LogConfig{"MI005_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["last_control_sec"] = LogConfig{"MI005_Monitoring_Data_1", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_version"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["serial_number"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_profile_crc"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["inverter_state"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_3"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_1"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_2"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_min"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_hour"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var_min"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["iac_rms"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vac_rms"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_frequency"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vpv"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["ipv"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["temperature"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_sub_version"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["lqi"] = LogConfig{"MI006_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["last_control_sec"] = LogConfig{"MI006_Monitoring_Data_1", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_version"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["serial_number"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_profile_crc"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["inverter_state"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_3"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_1"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_2"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_min"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_hour"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var_min"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["iac_rms"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vac_rms"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_frequency"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vpv"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["ipv"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["temperature"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_sub_version"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["lqi"] = LogConfig{"MI007_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["last_control_sec"] = LogConfig{"MI007_Monitoring_Data_1", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_version"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["serial_number"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_profile_crc"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["inverter_state"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_3"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_1"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_2"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_min"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_hour"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var_min"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["iac_rms"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vac_rms"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_frequency"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vpv"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["ipv"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["temperature"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_sub_version"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["lqi"] = LogConfig{"MI008_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["last_control_sec"] = LogConfig{"MI008_Monitoring_Data_1", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_version"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["serial_number"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_profile_crc"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["inverter_state"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_3"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_1"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["alarms_2"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_min"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt_hour"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var_min"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["watt"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["var"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["iac_rms"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vac_rms"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["grid_frequency"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["vpv"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["ipv"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["temperature"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["fw_sub_version"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["lqi"] = LogConfig{"MI009_Monitoring_Data_0", "MM", "MI_Monitoring.csv", 900, 86400, 8192};
    logMapping["last_control_sec"] = LogConfig{"MI009_Monitoring_Data_1", "MM", "MI_Monitoring.csv", 900, 86400, 8192};

    // Add all entries or load from JSON
}

bool LoggerUtils::shouldLog(const std::string& metricName) {
    auto&logMapping = logMapping_;
    std::lock_guard<std::mutex> lock(logMutex);
    auto now = std::chrono::system_clock::now();
    auto it = metricStates.find(metricName);

    if (it == metricStates.end()) {
        metricStates[metricName] = {now};
        return true;
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - it->second.lastLogged).count();
    int cycle = logMapping[metricName].writeCycleSeconds;

    if (elapsed >= cycle) {
        it->second.lastLogged = now;
        return true;
    }
    return false;
}

void LoggerUtils::checkAndRotateFile(const std::string& fileName) {
    auto&logMapping = logMapping_;
    auto now = std::chrono::system_clock::now();
    auto it = fileStates.find(fileName);

    if (it == fileStates.end()) {
        fileStates[fileName] = {now};
        return;
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - it->second.creationTime).count();
    int rotationTime = logMapping[fileName].rotationSeconds;

    if (elapsed >= rotationTime) {
        std::string srcPath = LOG_DIR + fileName;
        std::string destPath = UPLOAD_DIR + fileName;

        if (std::rename(srcPath.c_str(), destPath.c_str()) == 0) {
            std::cout << "Rotated log file: " << fileName << " to " << destPath << std::endl;
            fileStates[fileName].creationTime = now;
        }

        // Create .meta file
        std::string metaFilePath = destPath + ".meta";
        std::ofstream metaFile(metaFilePath);
        if (metaFile.is_open()) {
            auto t = std::chrono::system_clock::to_time_t(now);
            metaFile << "upload_name = " << "blob_storage_upload_name" << "\n";
            metaFile << "upload_path = " << logMapping[fileName].logType + "/" + logMapping[fileName].logFileName << "\n";
            metaFile << "post_action = " << "move" << "\n";
            metaFile << "move_dir = " << "uploaded/"  + logMapping[fileName].logFileName  << "\n";
            metaFile.close();
        }

    }
}



void LoggerUtils::logMetricToCSV(const std::string& metricName,
                                 const std::string& value,
                                 const std::string& fileName) {
    auto& logMapping = logMapping_;
    std::lock_guard<std::mutex> lock(logMutex);

    // Check if we should log this metric now
    //if (!shouldLog(metricName)) return;

    std::string filePath = LOG_DIR + fileName;

    // Create directory only if it doesn't exist
    if (!std::filesystem::exists(LOG_DIR)) {
        std::filesystem::create_directories(LOG_DIR);
    }

    bool fileExists = std::filesystem::exists(filePath);

    std::ofstream file(filePath, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open or create file: " << filePath << std::endl;
        return;
    }

    // Write header if file is new
    if (!fileExists) {
        file << "date,time,regID,metricName,value\n";
    }

    // Get current timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&t);

    char dateBuf[11];  // YYYY-MM-DD
    char timeBuf[9];   // HH:MM:SS
    std::strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", &localTime);
    std::strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &localTime);

    // Write log entry
    file << dateBuf << "," << timeBuf << ","  << logMapping[metricName].regId  << "," << metricName << "," << value  << "\n";
}

