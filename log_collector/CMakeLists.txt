cmake_minimum_required(VERSION 3.16)
project(log_collector)

# Set c++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set source and include paths
set(BASEAPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../uniep/core/common/base_app)
set(APPCORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../uniep/core/common/app_core)

set(COMMON_FLAGS "-fno-omit-frame-pointer -rdynamic -funwind-tables")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG   "-O2 -g1 ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG     "-O2 -g1 ${COMMON_FLAGS}")



# Define app name as a compile definition
add_compile_definitions(APP_NAME="${PROJECT_NAME}")

# Source files
add_executable(log_collector
    source/main.cpp
    source/logcollector_application.cpp
    source/logger_utils.cpp
    source/metric_loader.cpp
    ${BASEAPP_DIR}/source/base_app.cpp
    ${APPCORE_DIR}/source/edge_logger.cpp
    ${APPCORE_DIR}/source/edge_utils.cpp
    ${APPCORE_DIR}/source/edge_json.cpp
    ${APPCORE_DIR}/source/edge_backtrace.cpp
)

# Include directories
target_include_directories(log_collector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${BASEAPP_DIR}/include
    ${APPCORE_DIR}/include
    /usr/local/include
)

# jsoncpp
include_directories(/usr/include/jsoncpp)
link_directories(/usr/aarch64-linux-gnu/lib)
# Link to Paho MQTT C++ and C libraries
target_link_libraries(log_collector
    paho-mqttpp3
    paho-mqtt3as
    pthread
    jsoncpp
    backtrace
    uuid
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
# Sample to add files for application
# install(DIRECTORY files/
#     DESTINATION files/${PROJECT_NAME}
#     FILES_MATCHING
#         PATTERN "*.json"
#         PATTERN "*.txt"
# )
