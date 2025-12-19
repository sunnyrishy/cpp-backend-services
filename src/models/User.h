#pragma once

#include <string>
#include <json/json.h>

namespace models {

/**
 * User Model - Represents a user in the system
 */
struct User {
    std::string id;
    std::string name;
    std::string email;
    std::string createdAt;
    
    // Convert User to JSON
    Json::Value toJson() const {
        Json::Value json;
        json["id"] = id;
        json["name"] = name;
        json["email"] = email;
        json["createdAt"] = createdAt;
        return json;
    }
    
    // Create User from JSON
    static User fromJson(const Json::Value& json) {
        User user;
        user.id = json.get("id", "").asString();
        user.name = json.get("name", "").asString();
        user.email = json.get("email", "").asString();
        user.createdAt = json.get("createdAt", "").asString();
        return user;
    }
    
    // Validate user data
    static bool validate(const Json::Value& json, std::string& error) {
        if (!json.isMember("name") || json["name"].asString().empty()) {
            error = "Field 'name' is required and cannot be empty";
            return false;
        }
        
        if (!json.isMember("email") || json["email"].asString().empty()) {
            error = "Field 'email' is required and cannot be empty";
            return false;
        }
        
        // Basic email validation
        std::string email = json["email"].asString();
        if (email.find('@') == std::string::npos) {
            error = "Invalid email format";
            return false;
        }
        
        return true;
    }
};

} // namespace models