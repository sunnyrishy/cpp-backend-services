#pragma once

#include <json/json.h>
#include <string>

namespace models {

/**
 * ApiResponse - Standard API response structure
 */
class ApiResponse {
public:
    // Success response with data
    static Json::Value success(const std::string& message, const Json::Value& data = Json::Value::null) {
        Json::Value response;
        response["status"] = "success";
        response["message"] = message;
        if (!data.isNull()) {
            response["data"] = data;
        }
        return response;
    }
    
    // Error response
    static Json::Value error(const std::string& message, int code = 400) {
        Json::Value response;
        response["status"] = "error";
        response["message"] = message;
        response["code"] = code;
        return response;
    }
    
    // Success response with count
    static Json::Value successWithCount(const std::string& message, const Json::Value& data, int count) {
        Json::Value response = success(message, data);
        response["count"] = count;
        return response;
    }
};

} // namespace models