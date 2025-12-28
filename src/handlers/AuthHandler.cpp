#include "handlers/AuthHandler.h"
#include "models/User.h"
#include "models/ApiResponse.h"
#include "repositories/UserRepository.h"
#include "utils/PasswordHash.h"
#include "utils/JwtToken.h"
#include "exceptions/ApiException.h"

using namespace handlers;
using namespace models;
using namespace repositories;
using namespace myapp;
using namespace exceptions;

// POST /api/auth/register - Register new user
void handlers::handleRegister(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto jsonBody = req->getJsonObject();
    
    if (!jsonBody) {
        throw BadRequestException("Invalid JSON body");
    }
    
    // Validate registration data
    std::string validationError;
    if (!User::validateRegistration(*jsonBody, validationError)) {
        throw ValidationException(validationError);
    }
    
    std::string name = (*jsonBody)["name"].asString();
    std::string email = (*jsonBody)["email"].asString();
    std::string password = (*jsonBody)["password"].asString();
    
    // Validate password strength
    std::string passwordError = PasswordHash::validateStrength(password);
    if (!passwordError.empty()) {
        throw ValidationException(passwordError);
    }
    
    // Check if email already exists
    if (UserRepository::emailExists(email)) {
        throw ValidationException("Email already exists");
    }
    
    // Hash password
    std::string passwordHash = PasswordHash::hash(password);
    
    // Create user in database
    User newUser = UserRepository::createWithPassword(name, email, passwordHash);
    
    // Generate JWT token
    std::string token = JwtToken::generate(newUser.id, newUser.email);
    
    // Prepare response
    Json::Value responseData;
    responseData["user"] = newUser.toJson();
    responseData["token"] = token;
    
    Json::Value response = ApiResponse::success(
        "User registered successfully",
        responseData
    );
    
    auto resp = HttpResponse::newHttpJsonResponse(response);
    resp->setStatusCode(k201Created);
    callback(resp);
}

// POST /api/auth/login - Login user
void handlers::handleLogin(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto jsonBody = req->getJsonObject();
    
    if (!jsonBody) {
        throw BadRequestException("Invalid JSON body");
    }
    
    // Validate login data
    std::string validationError;
    if (!User::validateLogin(*jsonBody, validationError)) {
        throw ValidationException(validationError);
    }
    
    std::string email = (*jsonBody)["email"].asString();
    std::string password = (*jsonBody)["password"].asString();
    
    // Find user by email
    auto userOpt = UserRepository::findByEmail(email);
    
    if (!userOpt.has_value()) {
        throw UnauthorizedException("Invalid email or password");
    }
    
    User user = userOpt.value();
    
    // Verify password
    if (!PasswordHash::verify(password, user.passwordHash)) {
        throw UnauthorizedException("Invalid email or password");
    }
    
    // Generate JWT token
    std::string token = JwtToken::generate(user.id, user.email);
    
    // Prepare response
    Json::Value responseData;
    responseData["user"] = user.toJson();
    responseData["token"] = token;
    
    Json::Value response = ApiResponse::success(
        "Login successful",
        responseData
    );
    
    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

// GET /api/auth/me - Get current user (protected route)
void handlers::handleGetCurrentUser(const HttpRequestPtr &req,
                                   std::function<void(const HttpResponsePtr &)> &&callback)
{
    // Extract token from Authorization header
    std::string authHeader = req->getHeader("Authorization");
    
    if (authHeader.empty()) {
        throw UnauthorizedException("Authorization header missing");
    }
    
    // Check for "Bearer " prefix
    if (authHeader.substr(0, 7) != "Bearer ") {
        throw UnauthorizedException("Invalid authorization format. Use: Bearer <token>");
    }
    
    // Extract token
    std::string token = authHeader.substr(7);
    
    // Verify token
    std::string userId = JwtToken::verify(token);
    
    if (userId.empty()) {
        throw UnauthorizedException("Invalid or expired token");
    }
    
    // Get user from database
    auto userOpt = UserRepository::findById(userId);
    
    if (!userOpt.has_value()) {
        throw NotFoundException("User not found");
    }
    
    User user = userOpt.value();
    
    Json::Value response = ApiResponse::success(
        "User retrieved successfully",
        user.toJson()
    );
    
    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}