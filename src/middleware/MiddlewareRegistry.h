#pragma once

#include <drogon/HttpAppFramework.h>
#include "middleware/LoggerMiddleware.h"
#include "middleware/CorsMiddleware.h"

namespace middleware {

/**
 * MiddlewareRegistry - Central place to register all middleware
 */
class MiddlewareRegistry {
public:
    static void registerAll() {
        // Register global filters (apply to all routes)
        // Order matters: they execute in registration order
        
        // 1. CORS - must be first to handle OPTIONS requests
        drogon::app().registerFilter(std::make_shared<CorsMiddleware>());
        
        // 2. Logger - logs all requests
        drogon::app().registerFilter(std::make_shared<LoggerMiddleware>());
        
        std::cout << "✅ Middleware registered successfully" << std::endl;
        std::cout << "   → CORS Middleware" << std::endl;
        std::cout << "   → Logger Middleware" << std::endl;
    }
};

} // namespace middleware