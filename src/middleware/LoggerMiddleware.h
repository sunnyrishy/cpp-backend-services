#pragma once

#include <drogon/HttpFilter.h>
#include <chrono>

using namespace drogon;

namespace middleware {

/**
 * LoggerMiddleware - Logs all incoming requests
 * Records: method, path, IP, timestamp, response time
 */
class LoggerMiddleware : public HttpFilter<LoggerMiddleware, false> {  // false = disable auto-creation
public:
    LoggerMiddleware() {}
    
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};

} // namespace middleware