#include "handlers/HealthHandler.h"
#include "models/ApiResponse.h"
#include <chrono>

using namespace handlers;
using namespace models;

// Basic health check
void handlers::handleHealthCheck(const HttpRequestPtr &req,
                                 std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()
    ).count();
    
    Json::Value response;
    response["status"] = "healthy";
    response["timestamp"] = static_cast<Json::Int64>(timestamp);
    response["service"] = "cpp-backend-services";
    response["version"] = "1.0.0";
    
    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

// Database health check
void handlers::handleDatabaseHealthCheck(const HttpRequestPtr &req,
                                        std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value response;
    
    try {
        // Test database connection
        bool dbHealthy = database::DatabaseManager::getInstance().testConnection();
        
        if (dbHealthy) {
            response["status"] = "healthy";
            response["database"] = "connected";
            response["message"] = "Database connection is healthy";
            
            auto resp = HttpResponse::newHttpJsonResponse(response);
            callback(resp);
        } else {
            response["status"] = "unhealthy";
            response["database"] = "disconnected";
            response["message"] = "Database connection failed";
            
            auto resp = HttpResponse::newHttpJsonResponse(response);
            resp->setStatusCode(k503ServiceUnavailable);
            callback(resp);
        }
    } catch (const std::exception& e) {
        response["status"] = "unhealthy";
        response["database"] = "error";
        response["message"] = e.what();
        
        auto resp = HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(k503ServiceUnavailable);
        callback(resp);
    }
}