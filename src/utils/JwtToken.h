#pragma once

#include <string>
#include <jwt-cpp/jwt.h>
#include <chrono>
#include <iostream>

namespace myapp {

/**
 * JwtToken - Utility for creating and validating JWT tokens
 */
class JwtToken {
private:
    // Secret key for signing tokens (in production, use environment variable)
    static constexpr const char* SECRET_KEY = "your-secret-key-change-this-in-production-12345";
    
    // Token expiration time (24 hours)
    static constexpr int EXPIRATION_HOURS = 24;

public:
    /**
     * Generate JWT token for a user
     */
    static std::string generate(const std::string& userId, const std::string& email) {
        auto now = std::chrono::system_clock::now();
        auto exp = now + std::chrono::hours(EXPIRATION_HOURS);
        
        try {
            auto token = jwt::create()
                .set_issuer("cpp-backend-services")
                .set_type("JWT")
                .set_issued_at(now)
                .set_expires_at(exp)
                .set_payload_claim("userId", jwt::claim(userId))
                .set_payload_claim("email", jwt::claim(email))
                .sign(jwt::algorithm::hs256{SECRET_KEY});
            
            std::cout << "✅ JWT token generated for user: " << userId << std::endl;
            return token;
            
        } catch (const std::exception& e) {
            std::cerr << "❌ Error generating JWT: " << e.what() << std::endl;
            throw;
        }
    }
    
    /**
     * Verify and decode JWT token
     * Returns userId if valid, empty string if invalid
     */
    static std::string verify(const std::string& token) {
        try {
            auto decoded = jwt::decode(token);
            
            auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{SECRET_KEY})
                .with_issuer("cpp-backend-services");
            
            verifier.verify(decoded);
            
            // Token is valid, extract userId
            std::string userId = decoded.get_payload_claim("userId").as_string();
            std::cout << "✅ JWT token verified for user: " << userId << std::endl;
            
            return userId;
            
        } catch (const std::exception& e) {
            std::cerr << "❌ JWT verification failed: " << e.what() << std::endl;
            return "";
        }
    }
    
    /**
     * Check if token is expired
     */
    static bool isExpired(const std::string& token) {
        try {
            auto decoded = jwt::decode(token);
            auto exp = decoded.get_expires_at();
            auto now = std::chrono::system_clock::now();
            
            return exp < now;
            
        } catch (const std::exception& e) {
            return true;
        }
    }
    
    /**
     * Extract email from token (without verification)
     */
    static std::string getEmail(const std::string& token) {
        try {
            auto decoded = jwt::decode(token);
            return decoded.get_payload_claim("email").as_string();
        } catch (const std::exception& e) {
            return "";
        }
    }
};

} // namespace myapp