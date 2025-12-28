# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(cpp-backend-services CXX)

# Use C++17 standard (modern C++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type (Debug mode for now - gives better error messages)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Find required packages
find_package(Drogon REQUIRED)
find_package(PkgConfig REQUIRED)
# Find OpenSSL for JWT
find_package(OpenSSL REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

# Include directories (where header files are)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PQXX_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(/usr/local/include)
include_directories(/usr/local/include/bcrypt)

# Collect all source files RECURSIVELY
file(GLOB_RECURSE SRC_FILES 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Print all found source files (for debugging)
message(STATUS "Found source files:")
foreach(SRC_FILE ${SRC_FILES})
    message(STATUS "  ${SRC_FILE}")
endforeach()

# Create the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Drogon::Drogon
    ${PQXX_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    bcrypt
)

# Add library directories
link_directories(${PQXX_LIBRARY_DIRS})

# Print build info (helpful for debugging)
message(STATUS "Building ${PROJECT_NAME} in ${CMAKE_BUILD_TYPE} mode")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "libpqxx version: ${PQXX_VERSION}")