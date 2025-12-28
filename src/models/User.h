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
    std::string passwordHash;  // NEW: Store hashed password
    std::string createdAt;
    
    // Convert User to JSON (WITHOUT password)
    Json::Value toJson() const {
        Json::Value json;
        json["id"] = id;
        json["name"] = name;
        json["email"] = email;
        json["createdAt"] = createdAt;
        // NOTE: Never send passwordHash to client!
        return json;
    }
    
    // Create User from JSON
    static User fromJson(const Json::Value& json) {
        User user;
        user.id = json.get("id", "").asString();
        user.name = json.get("name", "").asString();
        user.email = json.get("email", "").asString();
        user.createdAt = json.get("createdAt", "").asString();
        // passwordHash should never come from client JSON
        return user;
    }
    
    // Validate user data for registration
    static bool validateRegistration(const Json::Value& json, std::string& error) {
        // Check name
        if (!json.isMember("name") || json["name"].asString().empty()) {
            error = "Field 'name' is required and cannot be empty";
            return false;
        }
        
        if (json["name"].asString().length() > 100) {
            error = "Name must be less than 100 characters";
            return false;
        }
        
        // Check email
        if (!json.isMember("email") || json["email"].asString().empty()) {
            error = "Field 'email' is required and cannot be empty";
            return false;
        }
        
        std::string email = json["email"].asString();
        if (email.find('@') == std::string::npos) {
            error = "Invalid email format";
            return false;
        }
        
        if (email.length() > 255) {
            error = "Email must be less than 255 characters";
            return false;
        }
        
        // Check password
        if (!json.isMember("password") || json["password"].asString().empty()) {
            error = "Field 'password' is required and cannot be empty";
            return false;
        }
        
        return true;
    }
    
    // Validate login data
    static bool validateLogin(const Json::Value& json, std::string& error) {
        if (!json.isMember("email") || json["email"].asString().empty()) {
            error = "Email is required";
            return false;
        }
        
        if (!json.isMember("password") || json["password"].asString().empty()) {
            error = "Password is required";
            return false;
        }
        
        return true;
    }
    
    // Old validate method (kept for backward compatibility with existing endpoints)
    static bool validate(const Json::Value& json, std::string& error) {
        if (!json.isMember("name") || json["name"].asString().empty()) {
            error = "Field 'name' is required and cannot be empty";
            return false;
        }
        
        if (!json.isMember("email") || json["email"].asString().empty()) {
            error = "Field 'email' is required and cannot be empty";
            return false;
        }
        
        std::string email = json["email"].asString();
        if (email.find('@') == std::string::npos) {
            error = "Invalid email format";
            return false;
        }
        
        return true;
    }
};

} // namespace models