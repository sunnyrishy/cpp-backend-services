#pragma once

#include <pqxx/pqxx>
#include <vector>
#include <optional>
#include "models/User.h"
#include "database/DatabaseManager.h"
#include "exceptions/ApiException.h"
#include <iostream>

namespace repositories {

/**
 * UserRepository - Handles all database operations for users
 */
class UserRepository {
public:
    /**
     * Get all users from database
     */
    static std::vector<models::User> findAll() {
        std::vector<models::User> users;
        
        try {
            auto conn = database::DatabaseManager::getInstance().getConnection();
            pqxx::work txn(*conn);
            
            auto result = txn.exec(
                "SELECT id, name, email, "
                "TO_CHAR(created_at, 'YYYY-MM-DD\"T\"HH24:MI:SS\"Z\"') as created_at "
                "FROM users ORDER BY id"
            );
            
            for (const auto& row : result) {
                models::User user;
                user.id = row["id"].as<std::string>();
                user.name = row["name"].as<std::string>();
                user.email = row["email"].as<std::string>();
                user.createdAt = row["created_at"].as<std::string>();
                users.push_back(user);
            }
            
            txn.commit();
            std::cout << "✅ Retrieved " << users.size() << " users from database" << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "❌ Database error in findAll: " << e.what() << std::endl;
            throw exceptions::InternalServerException("Failed to retrieve users from database");
        }
        
        return users;
    }
    
    /**
     * Find user by ID
     */
    static std::optional<models::User> findById(const std::string& id) {
        try {
            auto conn = database::DatabaseManager::getInstance().getConnection();
            pqxx::work txn(*conn);
            
            auto result = txn.exec_params(
                "SELECT id, name, email, "
                "TO_CHAR(created_at, 'YYYY-MM-DD\"T\"HH24:MI:SS\"Z\"') as created_at "
                "FROM users WHERE id = $1",
                id
            );
            
            if (result.empty()) {
                return std::nullopt;
            }
            
            models::User user;
            user.id = result[0]["id"].as<std::string>();
            user.name = result[0]["name"].as<std::string>();
            user.email = result[0]["email"].as<std::string>();
            user.createdAt = result[0]["created_at"].as<std::string>();
            
            txn.commit();
            std::cout << "✅ Found user with ID: " << id << std::endl;
            
            return user;
            
        } catch (const std::exception& e) {
            std::cerr << "❌ Database error in findById: " << e.what() << std::endl;
            throw exceptions::InternalServerException("Failed to retrieve user from database");
        }
    }
    
    /**
     * Create a new user
     */
    static models::User create(const std::string& name, const std::string& email) {
        try {
            auto conn = database::DatabaseManager::getInstance().getConnection();
            pqxx::work txn(*conn);
            
            auto result = txn.exec_params(
                "INSERT INTO users (name, email) "
                "VALUES ($1, $2) "
                "RETURNING id, name, email, "
                "TO_CHAR(created_at, 'YYYY-MM-DD\"T\"HH24:MI:SS\"Z\"') as created_at",
                name, email
            );
            
            models::User user;
            user.id = result[0]["id"].as<std::string>();
            user.name = result[0]["name"].as<std::string>();
            user.email = result[0]["email"].as<std::string>();
            user.createdAt = result[0]["created_at"].as<std::string>();
            
            txn.commit();
            std::cout << "✅ Created user with ID: " << user.id << std::endl;
            
            return user;
            
        } catch (const pqxx::unique_violation& e) {
            std::cerr << "❌ Duplicate email: " << email << std::endl;
            throw exceptions::ValidationException("Email already exists");
        } catch (const std::exception& e) {
            std::cerr << "❌ Database error in create: " << e.what() << std::endl;
            throw exceptions::InternalServerException("Failed to create user");
        }
    }
    
    /**
     * Check if email exists
     */
    static bool emailExists(const std::string& email) {
        try {
            auto conn = database::DatabaseManager::getInstance().getConnection();
            pqxx::work txn(*conn);
            
            auto result = txn.exec_params(
                "SELECT COUNT(*) as count FROM users WHERE email = $1",
                email
            );
            
            int count = result[0]["count"].as<int>();
            txn.commit();
            
            return count > 0;
            
        } catch (const std::exception& e) {
            std::cerr << "❌ Database error in emailExists: " << e.what() << std::endl;
            return false;
        }
    }
};

} // namespace repositories