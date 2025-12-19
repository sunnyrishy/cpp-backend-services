#pragma once

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <functional>

using namespace drogon;

namespace handlers {

// Handler declarations
void handleHome(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

void getAllUsers(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback);

void getUserById(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback,
                 std::string userId);

void createUser(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

} // namespace handlers