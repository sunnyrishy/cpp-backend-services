#pragma once

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include "exceptions/ApiException.h"
#include "models/ApiResponse.h"
#include <iostream>

using namespace drogon;

namespace myapp {

/**
 * ErrorHandler - Converts exceptions to proper HTTP responses
 */
class ErrorHandler {
public:
    // Handle ApiException
    static HttpResponsePtr handleApiException(const exceptions::ApiException& ex) {
        std::cout << "❌ API Exception: " << ex.getMessage() << std::endl;
        
        Json::Value response;
        response["status"] = "error";
        response["message"] = ex.getMessage();
        response["errorCode"] = ex.getErrorCode();
        
        auto resp = HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(static_cast<HttpStatusCode>(ex.getStatusCode()));
        return resp;
    }
    
    // Handle unknown exception
    static HttpResponsePtr handleUnknownException(const std::exception& ex) {
        std::cout << "❌ Unknown Exception: " << ex.what() << std::endl;
        
        Json::Value response;
        response["status"] = "error";
        response["message"] = "An unexpected error occurred";
        response["errorCode"] = "INTERNAL_SERVER_ERROR";
        
        auto resp = HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(k500InternalServerError);
        return resp;
    }
    
    // Wrap handler with exception handling
    template<typename HandlerFunc>
    static auto withErrorHandling(HandlerFunc handler) {
        return [handler](const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback) {
            try {
                handler(req, std::move(callback));
            } catch (const exceptions::ApiException& ex) {
                callback(handleApiException(ex));
            } catch (const std::exception& ex) {
                callback(handleUnknownException(ex));
            } catch (...) {
                std::cout << "❌ Unknown error caught" << std::endl;
                Json::Value response;
                response["status"] = "error";
                response["message"] = "An unexpected error occurred";
                response["errorCode"] = "UNKNOWN_ERROR";
                
                auto resp = HttpResponse::newHttpJsonResponse(response);
                resp->setStatusCode(k500InternalServerError);
                callback(resp);
            }
        };
    }
    
    // With error handling for handlers with path parameters
    template<typename HandlerFunc>
    static auto withErrorHandlingParam(HandlerFunc handler) {
        return [handler](const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback,
                        std::string param) {
            try {
                handler(req, std::move(callback), param);
            } catch (const exceptions::ApiException& ex) {
                callback(handleApiException(ex));
            } catch (const std::exception& ex) {
                callback(handleUnknownException(ex));
            } catch (...) {
                std::cout << "❌ Unknown error caught" << std::endl;
                Json::Value response;
                response["status"] = "error";
                response["message"] = "An unexpected error occurred";
                
                auto resp = HttpResponse::newHttpJsonResponse(response);
                resp->setStatusCode(k500InternalServerError);
                callback(resp);
            }
        };
    }
};

} // namespace myapp