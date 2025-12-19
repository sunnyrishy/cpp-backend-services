#include "handlers/UserHandlers.h"
#include "models/User.h"
#include "models/ApiResponse.h"
#include "exceptions/ApiException.h"
#include "repositories/UserRepository.h"

using namespace handlers;
using namespace models;
using namespace exceptions;
using namespace repositories;

// Handler: GET / - Home/Welcome
void handlers::handleHome(const HttpRequestPtr &req,
                         std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value endpoints = Json::Value(Json::arrayValue);
    endpoints.append("GET  /api/users - Get all users");
    endpoints.append("GET  /api/users/{id} - Get user by ID");
    endpoints.append("POST /api/users - Create new user");
    
    Json::Value response;
    response["status"] = "success";
    response["message"] = "C++ Backend API Server with PostgreSQL";
    response["version"] = "1.0.0";
    response["database"] = "Connected";
    response["endpoints"] = endpoints;
    
    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

// Handler: GET /api/users - Get all users FROM DATABASE
void handlers::getAllUsers(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback)
{
    // Fetch users from database
    std::vector<User> users = UserRepository::findAll();
    
    // Convert to JSON
    Json::Value usersJson(Json::arrayValue);
    for (const auto& user : users) {
        usersJson.append(user.toJson());
    }
    
    Json::Value response = ApiResponse::successWithCount(
        "Users retrieved successfully from database",
        usersJson,
        users.size()
    );
    
    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

// Handler: GET /api/users/{id} - Get user by ID FROM DATABASE
void handlers::getUserById(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback,
                          std::string userId)
{
    // Fetch user from database
    auto userOpt = UserRepository::findById(userId);
    
    if (!userOpt.has_value()) {
        throw NotFoundException("User with ID '" + userId + "' not found");
    }
    
    Json::Value response = ApiResponse::success(
        "User found",
        userOpt.value().toJson()
    );
    
    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

// Handler: POST /api/users - Create new user IN DATABASE
void handlers::createUser(const HttpRequestPtr &req,
                         std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto jsonBody = req->getJsonObject();
    
    if (!jsonBody) {
        throw BadRequestException("Invalid JSON body");
    }
    
    // Validate input
    std::string validationError;
    if (!User::validate(*jsonBody, validationError)) {
        throw ValidationException(validationError);
    }
    
    std::string name = (*jsonBody)["name"].asString();
    std::string email = (*jsonBody)["email"].asString();
    
    // Check if email already exists
    if (UserRepository::emailExists(email)) {
        throw ValidationException("Email '" + email + "' already exists");
    }
    
    // Create user in database
    User newUser = UserRepository::create(name, email);
    
    Json::Value response = ApiResponse::success(
        "User created successfully in database",
        newUser.toJson()
    );
    
    auto resp = HttpResponse::newHttpJsonResponse(response);
    resp->setStatusCode(k201Created);
    callback(resp);
}