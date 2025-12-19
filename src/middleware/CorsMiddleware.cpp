#include "middleware/CorsMiddleware.h"
#include <iostream>

using namespace middleware;

void CorsMiddleware::doFilter(const HttpRequestPtr &req,
                              FilterCallback &&fcb,
                              FilterChainCallback &&fccb)
{
    std::cout << "🌐 CORS Middleware: Processing request" << std::endl;
    
    // Handle preflight OPTIONS request
    if (req->getMethod() == Options) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        
        // Add CORS headers
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
        resp->addHeader("Access-Control-Max-Age", "3600");
        
        fcb(resp);
        return;
    }
    
    // For non-OPTIONS requests, continue to next filter/handler
    fccb();
}