#include <drogon/HttpAppFramework.h>
#include "handlers/UserHandlers.h"
#include "utils/RequestLogger.h"
#include "utils/ErrorHandler.h"
#include "database/DatabaseManager.h"
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include "handlers/HealthHandler.h"
#include "handlers/AuthHandler.h"

using namespace drogon;

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "🚀 C++ Backend Server Starting..." << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Create logs directory
    struct stat st = {0};
    if (stat("../logs", &st) == -1) {
        mkdir("../logs", 0755);
        std::cout << "📁 Created logs directory" << std::endl;
    }
    
    // Initialize database connection
    std::cout << "🔧 Initializing database connection..." << std::endl;
    database::DatabaseManager::initialize(
        "localhost",           // host
        "5432",               // port
        "cpp_backend_db",     // database name
        "cpp_backend_user",   // username
        "backend_pass_123"    // password
    );
    
    // Test database connection
    if (!database::DatabaseManager::getInstance().testConnection()) {
        std::cerr << "❌ Failed to connect to database. Exiting..." << std::endl;
        return 1;
    }
    std::cout << std::endl;
    
    // Register routes with logging & error handling
    std::cout << "🔧 Registering routes with logging & error handling..." << std::endl;
    
    app().registerHandler(
        "/",
        myapp::RequestLogger::withLogging(
            myapp::ErrorHandler::withErrorHandling(handlers::handleHome)
        ),
        {Get}
    );
    
    app().registerHandler(
        "/api/users",
        myapp::RequestLogger::withLogging(
            myapp::ErrorHandler::withErrorHandling(handlers::getAllUsers)
        ),
        {Get}
    );
    
    app().registerHandler(
        "/api/users/{1}",
        myapp::RequestLogger::withLoggingParam(
            myapp::ErrorHandler::withErrorHandlingParam(handlers::getUserById)
        ),
        {Get}
    );
    
    app().registerHandler(
        "/api/users",
        myapp::RequestLogger::withLogging(
            myapp::ErrorHandler::withErrorHandling(handlers::createUser)
        ),
        {Post}
    );

    // Auth endpoints
    app().registerHandler(
        "/api/auth/register",
        myapp::RequestLogger::withLogging(
            myapp::ErrorHandler::withErrorHandling(handlers::handleRegister)
        ),
        {Post}
    );
    
    app().registerHandler(
        "/api/auth/login",
        myapp::RequestLogger::withLogging(
            myapp::ErrorHandler::withErrorHandling(handlers::handleLogin)
        ),
        {Post}
    );
    
    app().registerHandler(
        "/api/auth/me",
        myapp::RequestLogger::withLogging(
            myapp::ErrorHandler::withErrorHandling(handlers::handleGetCurrentUser)
        ),
        {Get}
    );
    
    std::cout << "✅ Routes registered with logging & error handling:" << std::endl;
    std::cout << "   → GET  / [Logging + Errors]" << std::endl;
    std::cout << "   → GET  /health [Logging + Errors]" << std::endl;
    std::cout << "   → GET  /health/database [Logging + Errors]" << std::endl;
    std::cout << "   → POST /api/auth/register [Logging + Errors]" << std::endl;
    std::cout << "   → POST /api/auth/login [Logging + Errors]" << std::endl;
    std::cout << "   → GET  /api/auth/me [Logging + Errors]" << std::endl;
    std::cout << "   → GET  /api/users [Logging + Errors]" << std::endl;
    std::cout << "   → GET  /api/users/{id} [Logging + Errors]" << std::endl;
    std::cout << "   → POST /api/users [Logging + Errors]" << std::endl;
    std::cout << std::endl;

    // Health check endpoints
    app().registerHandler(
        "/health",
        myapp::RequestLogger::withLogging(
            myapp::ErrorHandler::withErrorHandling(handlers::handleHealthCheck)
        ),
        {Get}
    );
    
    app().registerHandler(
        "/health/database",
        myapp::RequestLogger::withLogging(
            myapp::ErrorHandler::withErrorHandling(handlers::handleDatabaseHealthCheck)
        ),
        {Get}
    );

    
    
    // Configure server
    app()
        .setLogPath("../logs")
        .setLogLevel(trantor::Logger::kInfo)
        .addListener("0.0.0.0", 8080)
        .setThreadNum(4);
    
    std::cout << "========================================" << std::endl;
    std::cout << "✅ Server Ready!" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "📡 Listening on: http://localhost:8080" << std::endl;
    std::cout << "🛑 Press Ctrl+C to stop" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Start server
    app().run();
    
    return 0;
}