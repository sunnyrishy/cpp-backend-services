#include "middleware/LoggerMiddleware.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace middleware;

void LoggerMiddleware::doFilter(const HttpRequestPtr &req,
                                FilterCallback &&fcb,
                                FilterChainCallback &&fccb)
{
    // Record start time
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Get request info
    std::string method = req->getMethodString();
    std::string path = req->getPath();
    std::string clientIp = req->getPeerAddr().toIp();
    
    // Get current timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    
    std::cout << "┌─────────────────────────────────────────────" << std::endl;
    std::cout << "│ 📥 Incoming Request" << std::endl;
    std::cout << "│ Time:   " << ss.str() << std::endl;
    std::cout << "│ Method: " << method << std::endl;
    std::cout << "│ Path:   " << path << std::endl;
    std::cout << "│ Client: " << clientIp << std::endl;
    
    // Log request headers (optional, useful for debugging)
    auto headers = req->getHeaders();
    if (!headers.empty()) {
        std::cout << "│ Headers:" << std::endl;
        for (const auto& header : headers) {
            std::cout << "│   " << header.first << ": " << header.second << std::endl;
        }
    }
    
    // Continue to next filter/handler
    fccb();
    
    // Calculate response time
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    std::cout << "│ ⏱️  Response Time: " << duration.count() << "ms" << std::endl;
    std::cout << "└─────────────────────────────────────────────" << std::endl;
    std::cout << std::endl;
}