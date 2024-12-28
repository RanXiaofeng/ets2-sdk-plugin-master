#pragma once

#include <memory>
#include <string>
#include "httplib.h"
#include "nlohmann/json.hpp"
#include "ets2-telemetry-common.hpp"

using json = nlohmann::json;

class WebAPI {
public:
    WebAPI(int port = 8080);
    ~WebAPI();

    void start();
    void stop();

private:
    void setupRoutes();
    
    // API 路由处理函数
    void handleTelemetryData(const httplib::Request& req, httplib::Response& res);
    void handleTruckData(const httplib::Request& req, httplib::Response& res);
    void handleTrailerData(const httplib::Request& req, httplib::Response& res);
    void handleJobData(const httplib::Request& req, httplib::Response& res);
    void handleNavigationData(const httplib::Request& req, httplib::Response& res);
    
    // 数据转换函数
    json convertTelemetryToJson(const ets2TelemetryMap_s* telemetry);
    json convertTruckToJson(const ets2TelemetryMap_s* telemetry);
    json convertTrailerToJson(const ets2TelemetryMap_s* telemetry);
    json convertJobToJson(const ets2TelemetryMap_s* telemetry);
    json convertNavigationToJson(const ets2TelemetryMap_s* telemetry);

    std::unique_ptr<httplib::Server> server_;
    int port_;
    bool running_;
}; 