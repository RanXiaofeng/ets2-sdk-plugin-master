#include "web_api.hpp"
#include "sharedmemory.hpp"
#include <iostream>

WebAPI::WebAPI(int port) : port_(port), running_(false) {
    server_ = std::make_unique<httplib::Server>();
}

WebAPI::~WebAPI() {
    stop();
}

void WebAPI::start() {
    if (running_) return;
    
    setupRoutes();
    
    std::cout << "Starting Web API server on port " << port_ << std::endl;
    running_ = true;
    server_->listen("0.0.0.0", port_);
}

void WebAPI::stop() {
    if (!running_) return;
    
    server_->stop();
    running_ = false;
    std::cout << "Web API server stopped" << std::endl;
}

void WebAPI::setupRoutes() {
    // 设置CORS headers
    server_->set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    // 注册所有路由
    server_->Get("/api/v1/telemetry", [this](const auto& req, auto& res) {
        handleTelemetryData(req, res);
    });
    
    server_->Get("/api/v1/truck", [this](const auto& req, auto& res) {
        handleTruckData(req, res);
    });
    
    server_->Get("/api/v1/trailer", [this](const auto& req, auto& res) {
        handleTrailerData(req, res);
    });
    
    server_->Get("/api/v1/job", [this](const auto& req, auto& res) {
        handleJobData(req, res);
    });
    
    server_->Get("/api/v1/navigation", [this](const auto& req, auto& res) {
        handleNavigationData(req, res);
    });
}

void WebAPI::handleTelemetryData(const httplib::Request& req, httplib::Response& res) {
    auto* telemetry = SharedMemory::getInstance().getTelemetryData();
    if (!telemetry) {
        res.status = 404;
        res.set_content("{\"error\":\"Telemetry data not available\"}", "application/json");
        return;
    }
    
    json response = convertTelemetryToJson(telemetry);
    res.set_content(response.dump(), "application/json");
}

void WebAPI::handleTruckData(const httplib::Request& req, httplib::Response& res) {
    auto* telemetry = SharedMemory::getInstance().getTelemetryData();
    if (!telemetry) {
        res.status = 404;
        res.set_content("{\"error\":\"Truck data not available\"}", "application/json");
        return;
    }
    
    json response = convertTruckToJson(telemetry);
    res.set_content(response.dump(), "application/json");
}

void WebAPI::handleTrailerData(const httplib::Request& req, httplib::Response& res) {
    auto* telemetry = SharedMemory::getInstance().getTelemetryData();
    if (!telemetry) {
        res.status = 404;
        res.set_content("{\"error\":\"Trailer data not available\"}", "application/json");
        return;
    }
    
    json response = convertTrailerToJson(telemetry);
    res.set_content(response.dump(), "application/json");
}

void WebAPI::handleJobData(const httplib::Request& req, httplib::Response& res) {
    auto* telemetry = SharedMemory::getInstance().getTelemetryData();
    if (!telemetry) {
        res.status = 404;
        res.set_content("{\"error\":\"Job data not available\"}", "application/json");
        return;
    }
    
    json response = convertJobToJson(telemetry);
    res.set_content(response.dump(), "application/json");
}

void WebAPI::handleNavigationData(const httplib::Request& req, httplib::Response& res) {
    auto* telemetry = SharedMemory::getInstance().getTelemetryData();
    if (!telemetry) {
        res.status = 404;
        res.set_content("{\"error\":\"Navigation data not available\"}", "application/json");
        return;
    }
    
    json response = convertNavigationToJson(telemetry);
    res.set_content(response.dump(), "application/json");
}

json WebAPI::convertTelemetryToJson(const ets2TelemetryMap_s* telemetry) {
    json j;
    
    // 基本信息
    j["time"] = telemetry->time;
    j["paused"] = telemetry->paused;
    
    // 版本信息
    j["version"] = {
        {"plugin_revision", telemetry->tel_revId.ets2_telemetry_plugin_revision},
        {"game_major", telemetry->tel_revId.ets2_version_major},
        {"game_minor", telemetry->tel_revId.ets2_version_minor}
    };
    
    // 车辆动态数据
    j["vehicle"] = {
        {"speed", telemetry->tel_rev1.speed},
        {"acceleration", {
            {"x", telemetry->tel_rev1.accelerationX},
            {"y", telemetry->tel_rev1.accelerationY},
            {"z", telemetry->tel_rev1.accelerationZ}
        }},
        {"position", {
            {"x", telemetry->tel_rev1.coordinateX},
            {"y", telemetry->tel_rev1.coordinateY},
            {"z", telemetry->tel_rev1.coordinateZ}
        }},
        {"rotation", {
            {"x", telemetry->tel_rev1.rotationX},
            {"y", telemetry->tel_rev1.rotationY},
            {"z", telemetry->tel_rev1.rotationZ}
        }}
    };
    
    return j;
}

json WebAPI::convertTruckToJson(const ets2TelemetryMap_s* telemetry) {
    json j;
    
    // 发动机数据
    j["engine"] = {
        {"enabled", telemetry->tel_rev3.engineEnabled},
        {"rpm", telemetry->tel_rev1.engineRpm},
        {"rpm_max", telemetry->tel_rev1.engineRpmMax},
        {"oil_pressure", telemetry->tel_rev3.oilPressure},
        {"oil_temperature", telemetry->tel_rev3.oilTemperature},
        {"water_temperature", telemetry->tel_rev3.waterTemperature},
        {"battery_voltage", telemetry->tel_rev3.batteryVoltage}
    };
    
    // 变速箱数据
    j["transmission"] = {
        {"gear", telemetry->tel_rev1.gear},
        {"gear_ranges", telemetry->tel_rev1.gearRanges},
        {"range_active", telemetry->tel_rev1.gearRangeActive},
        {"shifter_slot", telemetry->tel_rev3.shifterSlot}
    };
    
    // 燃料数据
    j["fuel"] = {
        {"amount", telemetry->tel_rev1.fuel},
        {"capacity", telemetry->tel_rev1.fuelCapacity},
        {"rate", telemetry->tel_rev1.fuelRate},
        {"avg_consumption", telemetry->tel_rev1.fuelAvgConsumption},
        {"warning", telemetry->tel_rev3.fuelWarning}
    };
    
    // 车辆状态
    j["status"] = {
        {"truck_weight", telemetry->tel_rev1.truckWeight},
        {"odometer", telemetry->tel_rev3.truckOdometer},
        {"cruise_control", {
            {"enabled", telemetry->tel_rev3.cruiseControl},
            {"speed", telemetry->tel_rev3.cruiseControlSpeed}
        }},
        {"wipers", telemetry->tel_rev3.wipers},
        {"park_brake", telemetry->tel_rev3.parkBrake},
        {"motor_brake", telemetry->tel_rev3.motorBrake},
        {"electric_enabled", telemetry->tel_rev3.electricEnabled},
        {"brake_temperature", telemetry->tel_rev3.brakeTemperature}
    };
    
    // 车辆信息
    j["info"] = {
        {"make", telemetry->tel_rev3.truckMake},
        {"make_id", telemetry->tel_rev3.truckMakeId},
        {"model", telemetry->tel_rev3.truckModel}
    };
    
    return j;
}

json WebAPI::convertTrailerToJson(const ets2TelemetryMap_s* telemetry) {
    json j;
    
    // 拖车基本信息
    j["attached"] = telemetry->tel_rev1.trailer_attached;
    j["id"] = telemetry->tel_rev2.trailerId;
    j["name"] = telemetry->tel_rev2.trailerName;
    
    // 拖车状态
    j["mass"] = telemetry->tel_rev2.trailerMass;
    j["wear"] = telemetry->tel_rev3.wearTrailer;
    
    // 拖车位置
    j["position"] = {
        {"x", telemetry->tel_rev4.trailerCoordinateX},
        {"y", telemetry->tel_rev4.trailerCoordinateY},
        {"z", telemetry->tel_rev4.trailerCoordinateZ}
    };
    
    j["rotation"] = {
        {"x", telemetry->tel_rev4.trailerRotationX},
        {"y", telemetry->tel_rev4.trailerRotationY},
        {"z", telemetry->tel_rev4.trailerRotationZ}
    };
    
    return j;
}

json WebAPI::convertJobToJson(const ets2TelemetryMap_s* telemetry) {
    json j;
    
    // 任务基本信息
    j["income"] = telemetry->tel_rev2.jobIncome;
    j["delivery_time"] = telemetry->tel_rev2.time_abs_delivery;
    
    // 起始地和目的地
    j["source"] = {
        {"city", telemetry->tel_rev2.citySrc},
        {"company", telemetry->tel_rev2.compSrc}
    };
    
    j["destination"] = {
        {"city", telemetry->tel_rev2.cityDst},
        {"company", telemetry->tel_rev2.compDst}
    };
    
    return j;
}

json WebAPI::convertNavigationToJson(const ets2TelemetryMap_s* telemetry) {
    json j;
    
    // 导航信息
    j["distance"] = telemetry->tel_rev4.navigationDistance;
    j["time"] = telemetry->tel_rev4.navigationTime;
    j["speed_limit"] = telemetry->tel_rev4.navigationSpeedLimit;
    j["next_rest_stop"] = telemetry->tel_rev4.nextRestStop;
    
    return j;
} 