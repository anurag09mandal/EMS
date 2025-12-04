
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
    logMapping["MCU_Boot_FW_VERSION"] = LogConfig{"MCU_Info", "EM", "EMSP_Maintenance.csv", 300, 21600, 8192};
    logMapping["MCU_FW_VERSION"] = LogConfig{"MCU_Info", "EM", "EMSP_Maintenance.csv", 300, 21600, 8192};
    logMapping["MCU_Startup_Status"] = LogConfig{"MCU_Status", "EM", "EMSP_Maintenance.csv", 300, 21600, 8192};
    logMapping["ConDevice_Install_SeqStatus"] = LogConfig{"MCU_Status", "EI", "EMSP_Installation.csv", 300, 21600, 8192};
    logMapping["MCU_Operation_Time"] = LogConfig{"MCU_Status", "EM", "EMSP_Maintenance.csv", 300, 21600, 8192};
    // logMapping["EMSP_HW_Reset_Count"] = LogConfig{"MCU_Maintenance", "EM", "EMSP_Maintenance.csv", 300, 21600, 8192};
    // logMapping["Set_Grid_Profile_Count"] = LogConfig{"MCU_Maintenance", "EM", "EMSP_Maintenance.csv", 300, 21600, 8192};
    // logMapping["EMSP_Mac_Frmae_Counter_Count"] = LogConfig{"MCU_Maintenance", "EM", "EMSP_Maintenance.csv", 300, 21600, 8192};
    // logMapping["MI_Mac_Frmae_Counter_Count"] = LogConfig{"MCU_Maintenance", "EM", "EMSP_Maintenance.csv", 300, 21600, 8192};
    // logMapping["EEPROM_Update_Count"] = LogConfig{"MCU_Maintenance", "EM", "EMSP_Maintenance.csv", 300, 21600, 8192};
    // logMapping["EEPROM_Error_Count"] = LogConfig{"MCU_Maintenance", "EM", "EMSP_Maintenance.csv", 300, 21600, 8192};
    // logMapping["Power_Control_Point"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Target_Power"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Battery_Target_Power"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PV_Power_Limit"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Battery_Installation_Status"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Feeder_Ex_P_Limit"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Feeder_Im_P_Limit"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Tot_ESS_to_Grid_Ex_P_Limit"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Tot_ESS_to_Grid_Im_P_Limit"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Tot_ESS_Ex_P_Limit"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Tot_ESS_Im_P_Limit"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Feed_In_Limit_Flag"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Feed_In_Limit"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Install_Flag1"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Install_Flag2"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag1"] = LogConfig{"PMU_Control_Data_01", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ESS1_Final_Target_P"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ESS2_Final_Target_P"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ESS3_Final_Target_P"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ESS4_Final_Target_P"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["MI_Power_Limit_Cmd"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Operation_Stop_Flag"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Device_Available_Flag"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ESS1_Operation_Cmd"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ESS2_Operation_Cmd"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ESS3_Operation_Cmd"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ESS4_Operation_Cmd"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["MI_Operation_Cmd"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["MPU_Fault_Release_Cmd"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["MSC_MID_Cmd"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ESS_Voltage_Ref"] = LogConfig{"PMU_Control_Data_02", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PMS_CMD"] = LogConfig{"PMS_CMD", "PMM", "PMU_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Grid_Fault_Flag0"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Fault_Flag1"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Fault_Flag2"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Fault_Flag3"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Fault_Flag4"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Fault_Flag5"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Fault_Flag6"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Fault_Flag7"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Warning_Flag0"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Warning_Flag1"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Warning_Flag2"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Warning_Flag3"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Warning_Flag4"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Warning_Flag5"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Warning_Flag6"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Warning_Flag7"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Alarm_Flag0"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Alarm_Flag1"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Alarm_Flag2"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Alarm_Flag3"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Alarm_Flag4"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Alarm_Flag5"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Alarm_Flag6"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Grid_Alarm_Flag7"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Fault_Flag0"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Fault_Flag1"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Fault_Flag2"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Fault_Flag3"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Fault_Flag4"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Fault_Flag5"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Fault_Flag6"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Fault_Flag7"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Warning_Flag0"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Warning_Flag1"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Warning_Flag2"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Warning_Flag3"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Warning_Flag4"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Warning_Flag5"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Warning_Flag6"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Warning_Flag7"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Alarm_Flag8"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Alarm_Flag9"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Alarm_Flag10"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Alarm_Flag11"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Alarm_Flag12"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Alarm_Flag13"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Alarm_Flag14"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["PCS_Alarm_Flag15"] = LogConfig{"PCS_Error_Status_1", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Fault_Flag0"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Fault_Flag1"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Fault_Flag2"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Fault_Flag3"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Fault_Flag4"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Fault_Flag5"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Fault_Flag6"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Fault_Flag7"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Warning_Flag0"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Warning_Flag1"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Warning_Flag2"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Warning_Flag3"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Warning_Flag4"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Warning_Flag5"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Warning_Flag6"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Warning_Flag7"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Alarm_Flag0"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Alarm_Flag1"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Alarm_Flag2"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Alarm_Flag3"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Alarm_Flag4"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Alarm_Flag5"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Alarm_Flag6"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["BDC_Alarm_Flag7"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Fault_Flag0"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Fault_Flag1"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Fault_Flag2"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Fault_Flag3"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Fault_Flag4"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Fault_Flag5"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Fault_Flag6"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Fault_Flag7"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Warning_Flag0"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Warning_Flag1"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Warning_Flag2"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Warning_Flag3"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Warning_Flag4"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Warning_Flag5"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Warning_Flag6"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Warning_Flag7"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Alarm_Flag0"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Alarm_Flag1"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Alarm_Flag2"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Alarm_Flag3"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Alarm_Flag4"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Alarm_Flag5"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Alarm_Flag6"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["MCU_Alarm_Flag7"] = LogConfig{"PCS_Error_Status_2", "PCF", "PCS_Fault.csv", 0, 0, 8192};
    // logMapping["Inv_Active_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Power_250ms"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Target_Active_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_Export_Power_Limit"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_Import_Power_Limit"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Grid_Code_Export_Power_Limit"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Grid_Code_Import_Power_Limit"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Grid_Apparent_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Grid_Active_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Grid_Reactive_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Grid_Power_Factor"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Grid_Apparent_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Grid_Active_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Grid_Reactive_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Grid_Power_Factor"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Inverter_Apparent_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Inverter_Active_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Inverter_Reactive_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Inverter_Power_Factor"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Inverter_Apparent_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Inverter_Active_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Inverter_Reactive_Power"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Inverter_Power_Factor"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Inverter_Voltage"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Inverter_Voltage"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Inverter_Voltage_DC_Offset"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Inverter_Voltage_DC_Offset"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Temperature_M1_S_LEG1"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Temperature_M1_S_LEG2"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Temperature_M2_S_LEG1"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Temperature_M2_S_LEG2"] = LogConfig{"PCS_Control_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PCS_CMD1"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Power_Control_Point"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["INV_Target_Power"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Battery_Target_Power"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_FWVer"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BootVer_BPU"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_HW_Ver"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Inverter_CPU1_Heartbeat"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Inverter_CPU1_Core_Area_Status"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Inverter_CPU2_Heartbeat"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Inverter_CPU2_Core_Area_Status"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Converter_CPU1_Heartbeat"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Converter_CPU1_Core_Area_Status"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Converter_CPU2_Heartbeat"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Converter_CPU2_Core_Area_Status"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ESS_Inverter_Frequency"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ESS_Grid_Frequency"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_CPU1_FWVer"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_CPU2_FWVer"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["INVERTER_CPU1_FWVer"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["INVERTER_CPU2_FWVer"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BootVer_BDC"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BootVer_INV"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PCS_HW_MainBoard_Ver"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PCS_HW_FilterBoard_Ver"] = LogConfig{"1s_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RS_PCS_Grid_Voltage"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Grid_Voltage"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Grid_Voltage"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_PCS_Grid_Current"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_PCS_Grid_Apparent_Power"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_PCS_Grid_Active_Power"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_PCS_Grid_Reactive_Power"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_PCS_Grid_Power_Factor"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RS_PCS_Inverter_Voltage"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["L1_PCS_Inverter_Current"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["L2_PCS_Inverter_Current"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_PCS_Inverter_Apparent_Power"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_RCMU_Current"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_PCS_Inverter_Active_Power"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_PCS_Inverter_Reactive_Power"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_PCS_Inverter_Power_Factor"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_PCS_Grid_Voltage_DC_Offset"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RN_PCS_Grid_Voltage_DC_Offset"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SN_PCS_Grid_Voltage_DC_Offset"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["RS_PCS_Inverter_Voltage_DC_Offset"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["L1_PCS_Inverter_Current_DC_Offset"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["L2_PCS_Inverter_Current_DC_Offset"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_RCMU_Current_DC_Offset"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SMPS_5V_Inverter"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SMPS_5V_Converter"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SMPS_24V_Inverter"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["SMPS_24V_Converter"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Power_1s"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Voltage_CONV"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Current1_CONV"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Current2_CONV"] = LogConfig{"1s_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BAT_Voltage_CONV"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["dclink_v_ref"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Current_Ref"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Power_Ref"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["INV_Temperature_LEG2"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["INV_Temperature_LEG4"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Temperature"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Temperature_M1_P_LEG1"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Temperature_M1_P_LEG2"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PCS_BDC_Operation_Seq_index"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PCS_INV_Operation_Seq_index"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PCS_Status"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PCS_Standby_Seq_Index"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PCS_Ready_Seq_Index"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PCS_Operation_Seq_Index"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PCS_Protection_Seq_Index"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DCLink_Voltage_INV_HL"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Charge_Power_Limit"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DCLink_Voltage_BDC_HL"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Discharge_Power_Limit"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DCLink_Voltage_INV_HM"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DCLink_Voltage_BDC_HM"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DCLink_Voltage_INV_ML"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DCLink_Voltage_BDC_ML"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Discharge_Current_Limit"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Charge_Current_Limit"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Total_Rack_Current"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["INV_Temperature_LEG5"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Temperature_M2_P_LEG1"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BDC_Temperature_M2_P_LEG2"] = LogConfig{"1s_Monitoring_Data_3", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["HUB_Relay_Status"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["Install_Check_Status"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["GEN_Freq"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["GEN_LL_V"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["GEN_Active_Power"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["GEN_Reactive_Power"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DER1_Active_Power"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DER1_Reactive_Power"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DER2_Active_Power"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DER2_Reactive_Power"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DER3_Active_Power"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DER3_Reactive_Power"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DER4_Active_Power"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["DER4_Reactive_Power"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["HUB_FWVer"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["HUB_BootVer"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["HUB_HW_MainBoard_Ver"] = LogConfig{"HUB_Control_Data", "AHM", "Advanced_HUB_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag0_0"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag0_1"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag0_2"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag0_3"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag0_4"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag0_5"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag0_6"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag0_7"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag1_0"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag1_1"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag1_2"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag1_3"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag1_4"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag1_5"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag1_6"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["System_Flag1_7"] = LogConfig{"PCS_Status_Data", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PcstoBpu_Relay_Close_CMD"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PcstoBpu_Relay_Open_CMD"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["PcstoBpu_Heating_Pad_CMD"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU1_Status"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU2_Status"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU3_Status"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU4_Status"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU1_Mode"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU2_Mode"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU3_Mode"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU4_Mode"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU1_Battery_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU2_Battery_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU3_Battery_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU4_Battery_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU1_DC_Bus_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU2_DC_Bus_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU3_DC_Bus_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU4_DC_Bus_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU1_SMPS_12V_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU2_SMPS_12V_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU1_SMPS_13V_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU2_SMPS_13V_Voltage"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU1_Temp"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU2_Temp"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU3_Temp"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU4_Temp"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU1_Current"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU2_Current"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU3_Current"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU4_Current"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU1_CT_Error_Info"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU2_CT_Error_Info"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU3_CT_Error_Info"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU4_CT_Error_Info"] = LogConfig{"BPU_Monitoring_Data_1", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU1_Power"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU2_Power"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU3_Power"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU4_Power"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Fault_Flag0"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Fault_Flag1"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Fault_Flag2"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Fault_Flag3"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Fault_Flag4"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Fault_Flag5"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Fault_Flag6"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Fault_Flag7"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Warning_Flag0"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Warning_Flag1"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Warning_Flag2"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Warning_Flag3"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Warning_Flag4"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Warning_Flag5"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Warning_Flag6"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Warning_Flag7"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Fault_Flag0"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Fault_Flag1"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Fault_Flag2"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Fault_Flag3"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Fault_Flag4"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Fault_Flag5"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Fault_Flag6"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Fault_Flag7"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Warning_Flag0"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Warning_Flag1"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Warning_Flag2"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Warning_Flag3"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Warning_Flag4"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Warning_Flag5"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Warning_Flag6"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_MCU_Warning_Flag7"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Main_P_Relay_Feedback"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["BPU_Main_N_Relay_Feedback"] = LogConfig{"BPU_Monitoring_Data_2", "PCCM", "PCS_CAN_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC1_Offset"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC2_Offset"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC3_Offset"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC4_Offset"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC5_Offset"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC6_Offset"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC7_Offset"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC8_Offset"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC9_Offset"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC10_Offset"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC11_Offset"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC1_Gain"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC2_Gain"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC3_Gain"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC4_Gain"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC5_Gain"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC6_Gain"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC7_Gain"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC8_Gain"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC9_Gain"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC10_Gain"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC11_Gain"] = LogConfig{"fault_Monitoring_Data_offset", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC1_Cal"] = LogConfig{"fault_Monitoring_Data_Gain", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC2_Cal"] = LogConfig{"fault_Monitoring_Data_Gain", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC2_Cal"] = LogConfig{"fault_Monitoring_Data_Gain", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC3_Cal"] = LogConfig{"fault_Monitoring_Data_Gain", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC4_Cal"] = LogConfig{"fault_Monitoring_Data_Gain", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC5_Cal"] = LogConfig{"fault_Monitoring_Data_Gain", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC6_Cal"] = LogConfig{"fault_Monitoring_Data_Gain", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC7_Cal"] = LogConfig{"fault_Monitoring_Data_Gain", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC8_Cal"] = LogConfig{"fault_Monitoring_Data_Gain", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC9_Cal"] = LogConfig{"fault_Monitoring_Data_Gain", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC1_Raw_Data"] = LogConfig{"fault_Monitoring_ADC_Data", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC2_Raw_Data"] = LogConfig{"fault_Monitoring_ADC_Data", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC3_Raw_Data"] = LogConfig{"fault_Monitoring_ADC_Data", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC4_Raw_Data"] = LogConfig{"fault_Monitoring_ADC_Data", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC5_Raw_Data"] = LogConfig{"fault_Monitoring_ADC_Data", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC6_Raw_Data"] = LogConfig{"fault_Monitoring_ADC_Data", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC7_Raw_Data"] = LogConfig{"fault_Monitoring_ADC_Data", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC8_Raw_Data"] = LogConfig{"fault_Monitoring_ADC_Data", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC9_Raw_Data"] = LogConfig{"fault_Monitoring_ADC_Data", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC10_Raw_Data"] = LogConfig{"fault_Monitoring_ADC_Data", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
    // logMapping["ADC11_Raw_Data"] = LogConfig{"fault_Monitoring_ADC_Data", "PCFM", "PCS_Fault_Monitoring.csv", 60, 21600, 8192};
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


//TODO: Filename should name with correct format as in log policy
void LoggerUtils::logMetricToCSV(const std::string& metricName,
                                 const std::string& value,
                                 const std::string& fileName) {
    auto& logMapping = logMapping_;
    std::lock_guard<std::mutex> lock(logMutex);

    // Check if we should log this metric now
    //if (!shouldLog(metricName)) return;

 // Get current timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&t);

    char dateBuf[11];  // YYYY-MM-DD
    char timeBuf[9];   // HH:MM:SS

    std::strftime(dateBuf, sizeof(dateBuf), "%Y%m%d", &localTime);
    std::strftime(timeBuf, sizeof(timeBuf), "%H%M%S", &localTime);



    std::string filePath = LOG_DIR + std::string(dateBuf) + "_" +  std::string(timeBuf) + "_" + fileName;//TODO: + "_"append serial number at end

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

   
    
    // Write log entry
    file << dateBuf << "," << timeBuf << ","  << logMapping[metricName].regId  << "," << metricName << "," << value  << "\n";
}

