#pragma once

#include <string>
#include <bcrypt.h>

namespace myapp {

/**
 * PasswordHash - Utility for password hashing and verification
 */
class PasswordHash {
public:
    /**
     * Hash a plain text password
     */
    static std::string hash(const std::string& password) {
        return bcrypt::generateHash(password);
    }
    
    /**
     * Verify a password against a hash
     */
    static bool verify(const std::string& password, const std::string& hash) {
        return bcrypt::validatePassword(password, hash);
    }
    
    /**
     * Validate password strength
     * Returns empty string if valid, error message if invalid
     */
    static std::string validateStrength(const std::string& password) {
        if (password.length() < 8) {
            return "Password must be at least 8 characters long";
        }
        
        if (password.length() > 100) {
            return "Password must be less than 100 characters";
        }
        
        // Check for at least one number
        bool hasNumber = false;
        bool hasLetter = false;
        
        for (char c : password) {
            if (std::isdigit(c)) hasNumber = true;
            if (std::isalpha(c)) hasLetter = true;
        }
        
        if (!hasNumber) {
            return "Password must contain at least one number";
        }
        
        if (!hasLetter) {
            return "Password must contain at least one letter";
        }
        
        return ""; // Valid
    }
};

} // namespace myapp