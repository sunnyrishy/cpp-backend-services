#pragma once

#include <drogon/HttpFilter.h>

using namespace drogon;

namespace middleware {

/**
 * CorsMiddleware - Handles Cross-Origin Resource Sharing (CORS)
 * Allows frontend apps to call this API from different domains
 */
class CorsMiddleware : public HttpFilter<CorsMiddleware, false> {  // false = disable auto-creation
public:
    CorsMiddleware() {}
    
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};

} // namespace middleware