#pragma once

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <functional>

using namespace drogon;

namespace handlers {

/**
 * AuthHandler - Authentication endpoints
 */

// POST /api/auth/register - Register new user
void handleRegister(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback);

// POST /api/auth/login - Login user
void handleLogin(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

// GET /api/auth/me - Get current user (protected route)
void handleGetCurrentUser(const HttpRequestPtr &req,
                         std::function<void(const HttpResponsePtr &)> &&callback);

} // namespace handlers