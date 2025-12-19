#pragma once

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <functional>

using namespace drogon;

namespace myapp {  // CHANGED from 'utils'

/**
 * RequestLogger - Simple request logging utility
 */
class RequestLogger {
public:
    // Wrap a handler with logging
    template<typename HandlerFunc>
    static auto withLogging(HandlerFunc handler) {
        return [handler](const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback) {
            
            // Start timing
            auto startTime = std::chrono::high_resolution_clock::now();
            
            // Get request info
            std::string method = req->getMethodString();
            std::string path = req->getPath();
            std::string clientIp = req->getPeerAddr().toIp();
            
            // Get timestamp
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
            
            // Log request
            std::cout << "┌─────────────────────────────────────────────" << std::endl;
            std::cout << "│ 📥 Incoming Request" << std::endl;
            std::cout << "│ Time:   " << ss.str() << std::endl;
            std::cout << "│ Method: " << method << std::endl;
            std::cout << "│ Path:   " << path << std::endl;
            std::cout << "│ Client: " << clientIp << std::endl;
            
            // Log headers
            auto headers = req->getHeaders();
            if (!headers.empty()) {
                std::cout << "│ Headers:" << std::endl;
                for (const auto& header : headers) {
                    std::cout << "│   " << header.first << ": " << header.second << std::endl;
                }
            }
            std::cout << "├─────────────────────────────────────────────" << std::endl;
            
            // Call the actual handler
            handler(req, [callback, startTime](const HttpResponsePtr &resp) {
                // Calculate response time
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
                
                std::cout << "│ ✅ Response sent" << std::endl;
                std::cout << "│ ⏱️  Response Time: " << duration.count() << "ms" << std::endl;
                std::cout << "└─────────────────────────────────────────────" << std::endl;
                std::cout << std::endl;
                
                callback(resp);
            });
        };
    }
    
    // For handlers with path parameters
    template<typename HandlerFunc>
    static auto withLoggingParam(HandlerFunc handler) {
        return [handler](const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback,
                        std::string param) {
            
            // Start timing
            auto startTime = std::chrono::high_resolution_clock::now();
            
            // Get request info
            std::string method = req->getMethodString();
            std::string path = req->getPath();
            std::string clientIp = req->getPeerAddr().toIp();
            
            // Get timestamp
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
            
            // Log request
            std::cout << "┌─────────────────────────────────────────────" << std::endl;
            std::cout << "│ 📥 Incoming Request" << std::endl;
            std::cout << "│ Time:   " << ss.str() << std::endl;
            std::cout << "│ Method: " << method << std::endl;
            std::cout << "│ Path:   " << path << std::endl;
            std::cout << "│ Param:  " << param << std::endl;
            std::cout << "│ Client: " << clientIp << std::endl;
            std::cout << "├─────────────────────────────────────────────" << std::endl;
            
            // Call the actual handler
            handler(req, [callback, startTime](const HttpResponsePtr &resp) {
                // Calculate response time
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
                
                std::cout << "│ ✅ Response sent" << std::endl;
                std::cout << "│ ⏱️  Response Time: " << duration.count() << "ms" << std::endl;
                std::cout << "└─────────────────────────────────────────────" << std::endl;
                std::cout << std::endl;
                
                callback(resp);
            }, param);
        };
    }
};

} // namespace myapp