#pragma once

#include <cstdlib>  // for getenv
#include <pqxx/pqxx>
#include <memory>
#include <string>
#include <iostream>

namespace database {

/**
 * DatabaseManager - Manages PostgreSQL connections
 * Singleton pattern for application-wide database access
 */
class DatabaseManager {
private:
    std::string connectionString_;
    
    // Singleton instance
    static DatabaseManager* instance_;
    
    // Private constructor for singleton
    DatabaseManager(const std::string& host, 
                   const std::string& port,
                   const std::string& dbname,
                   const std::string& user,
                   const std::string& password) {
        
        // Build PostgreSQL connection string
        connectionString_ = "host=" + host + 
                          " port=" + port +
                          " dbname=" + dbname + 
                          " user=" + user + 
                          " password=" + password;
        
        std::cout << "🔗 Database connection string configured" << std::endl;
    }

public:
    // Delete copy constructor and assignment operator (singleton)
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    
    /**
     * Initialize the singleton instance
     */
    static void initialize(const std::string& host = "",
                      const std::string& port = "5432",
                      const std::string& dbname = "cpp_backend_db",
                      const std::string& user = "cpp_backend_user",
                      const std::string& password = "backend_pass_123") {
    if (instance_ == nullptr) {
        // Read from environment variables if provided, otherwise use defaults
        std::string finalHost = host;
        if (finalHost.empty()) {
            const char* envHost = std::getenv("DB_HOST");
            finalHost = envHost ? envHost : "localhost";
        }
        
        std::string finalPort = port;
        const char* envPort = std::getenv("DB_PORT");
        if (envPort) finalPort = envPort;
        
        std::string finalDbname = dbname;
        const char* envDbname = std::getenv("DB_NAME");
        if (envDbname) finalDbname = envDbname;
        
        std::string finalUser = user;
        const char* envUser = std::getenv("DB_USER");
        if (envUser) finalUser = envUser;
        
        std::string finalPassword = password;
        const char* envPassword = std::getenv("DB_PASSWORD");
        if (envPassword) finalPassword = envPassword;
        
        instance_ = new DatabaseManager(finalHost, finalPort, finalDbname, finalUser, finalPassword);
        std::cout << "✅ DatabaseManager initialized (Host: " << finalHost << ")" << std::endl;
    }
}
    /**
     * Get the singleton instance
     */
    static DatabaseManager& getInstance() {
        if (instance_ == nullptr) {
            throw std::runtime_error("DatabaseManager not initialized! Call initialize() first.");
        }
        return *instance_;
    }
    
    /**
     * Get a new database connection
     */
    std::unique_ptr<pqxx::connection> getConnection() {
        try {
            auto conn = std::make_unique<pqxx::connection>(connectionString_);
            
            if (!conn->is_open()) {
                throw std::runtime_error("Failed to open database connection");
            }
            
            return conn;
        } catch (const std::exception& e) {
            std::cerr << "❌ Database connection error: " << e.what() << std::endl;
            throw;
        }
    }
    
    /**
     * Test database connection
     */
    bool testConnection() {
        try {
            auto conn = getConnection();
            pqxx::work txn(*conn);
            
            auto result = txn.exec("SELECT version()");
            txn.commit();
            
            std::cout << "✅ Database connection test successful" << std::endl;
            std::cout << "   PostgreSQL version: " << result[0][0].c_str() << std::endl;
            
            return true;
        } catch (const std::exception& e) {
            std::cerr << "❌ Database connection test failed: " << e.what() << std::endl;
            return false;
        }
    }
    
    /**
     * Get connection string (for debugging)
     */
    std::string getConnectionString() const {
        // Return masked version for security
        return "host=localhost port=5432 dbname=cpp_backend_db user=cpp_backend_user password=****";
    }
};

} // namespace database