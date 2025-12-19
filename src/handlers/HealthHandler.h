#pragma once

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include "database/DatabaseManager.h"
#include <functional>

using namespace drogon;

namespace handlers {

/**
 * HealthHandler - Health check endpoints
 */
void handleHealthCheck(const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&callback);

void handleDatabaseHealthCheck(const HttpRequestPtr &req,
                               std::function<void(const HttpResponsePtr &)> &&callback);

} // namespace handlers