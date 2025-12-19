#pragma once

#include <exception>
#include <string>
#include <drogon/HttpTypes.h>

namespace exceptions {

/**
 * Base API Exception - All custom exceptions inherit from this
 */
class ApiException : public std::exception {
protected:
    std::string message_;
    int statusCode_;
    std::string errorCode_;

public:
    ApiException(const std::string& message, 
                 int statusCode = 500,
                 const std::string& errorCode = "INTERNAL_ERROR")
        : message_(message), statusCode_(statusCode), errorCode_(errorCode) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

    int getStatusCode() const { return statusCode_; }
    std::string getErrorCode() const { return errorCode_; }
    std::string getMessage() const { return message_; }
};

/**
 * BadRequestException - 400 errors
 */
class BadRequestException : public ApiException {
public:
    BadRequestException(const std::string& message)
        : ApiException(message, 400, "BAD_REQUEST") {}
};

/**
 * NotFoundException - 404 errors
 */
class NotFoundException : public ApiException {
public:
    NotFoundException(const std::string& message)
        : ApiException(message, 404, "NOT_FOUND") {}
};

/**
 * UnauthorizedException - 401 errors
 */
class UnauthorizedException : public ApiException {
public:
    UnauthorizedException(const std::string& message)
        : ApiException(message, 401, "UNAUTHORIZED") {}
};

/**
 * ValidationException - 422 errors
 */
class ValidationException : public ApiException {
public:
    ValidationException(const std::string& message)
        : ApiException(message, 422, "VALIDATION_ERROR") {}
};

/**
 * InternalServerException - 500 errors
 */
class InternalServerException : public ApiException {
public:
    InternalServerException(const std::string& message)
        : ApiException(message, 500, "INTERNAL_SERVER_ERROR") {}
};

} // namespace exceptions