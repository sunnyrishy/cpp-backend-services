# C++ Backend Services - Production-Ready REST API

A modern, production-ready REST API backend built with C++17, Drogon framework, and PostgreSQL.

## 🚀 Features

- ✅ RESTful API endpoints
- ✅ PostgreSQL database integration
- ✅ Clean architecture (handlers, repositories, models)
- ✅ Request logging with timing
- ✅ Global error handling
- ✅ Custom exceptions
- ✅ Data validation
- ✅ Health check endpoints
- ✅ JSON request/response
- ✅ Connection pooling

## 🛠️ Tech Stack

- **Language**: C++17
- **Framework**: Drogon (High-performance web framework)
- **Database**: PostgreSQL 16
- **Build System**: CMake
- **JSON Library**: JsonCpp
- **Database Library**: libpqxx

## 📋 Prerequisites

- Ubuntu 24.04 (or WSL2 with Ubuntu)
- g++ 13.x or later
- CMake 3.10 or later
- PostgreSQL 16
- Drogon framework

## 🔧 Installation

### 1. Install Dependencies
```bash
# Update packages
sudo apt update

# Install build tools
sudo apt install -y build-essential cmake git

# Install PostgreSQL
sudo apt install -y postgresql postgresql-contrib libpq-dev

# Install libpqxx
sudo apt install -y libpqxx-dev
```

### 2. Install Drogon Framework
```bash
cd ~
git clone https://github.com/drogonframework/drogon.git
cd drogon
git submodule init
git submodule update
mkdir build && cd build
cmake ..
make -j4
sudo make install
sudo ldconfig
```

### 3. Setup Database
```bash
# Start PostgreSQL
sudo service postgresql start

# Create database and user
psql -U postgres -h localhost << EOF
CREATE DATABASE cpp_backend_db;
CREATE USER cpp_backend_user WITH PASSWORD 'backend_pass_123';
GRANT ALL PRIVILEGES ON DATABASE cpp_backend_db TO cpp_backend_user;
\c cpp_backend_db
GRANT ALL ON SCHEMA public TO cpp_backend_user;
EOF

# Create tables
psql -U cpp_backend_user -h localhost -d cpp_backend_db << EOF
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(255) NOT NULL UNIQUE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_users_email ON users(email);

INSERT INTO users (name, email) VALUES 
    ('John Doe', 'john@example.com'),
    ('Jane Smith', 'jane@example.com'),
    ('Alice Johnson', 'alice@example.com');
EOF
```

### 4. Build the Project
```bash
cd ~/cpp-backend-services
mkdir build && cd build
cmake ..
make -j4
```

### 5. Run the Server
```bash
./cpp-backend-services
```

Or use the startup script:
```bash
cd ~/cpp-backend-services
./start-server.sh
```

## 📡 API Endpoints

### Base URL: `http://localhost:8080`

| Method | Endpoint | Description | Auth Required |
|--------|----------|-------------|---------------|
| GET | `/` | API information | No |
| GET | `/health` | Basic health check | No |
| GET | `/health/database` | Database health check | No |
| **POST** | **`/api/auth/register`** | **Register new user** | **No** |
| **POST** | **`/api/auth/login`** | **Login user** | **No** |
| **GET** | **`/api/auth/me`** | **Get current user** | **Yes** |
| GET | `/api/users` | Get all users | No |
| GET | `/api/users/{id}` | Get user by ID | No |
| POST | `/api/users` | Create new user | No |

## 🔐 Authentication

This API uses **JWT (JSON Web Tokens)** for authentication.

### Register a New User
```bash
curl -X POST http://localhost:8080/api/auth/register \
  -H "Content-Type: application/json" \
  -d '{
    "name": "John Doe",
    "email": "john@example.com",
    "password": "SecurePass123"
  }'
```

**Response:**
```json
{
  "status": "success",
  "message": "User registered successfully",
  "data": {
    "user": {
      "id": "1",
      "name": "John Doe",
      "email": "john@example.com",
      "createdAt": "2024-12-19T10:00:00Z"
    },
    "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
  }
}
```

### Login
```bash
curl -X POST http://localhost:8080/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{
    "email": "john@example.com",
    "password": "SecurePass123"
  }'
```

**Response:**
```json
{
  "status": "success",
  "message": "Login successful",
  "data": {
    "user": { ... },
    "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
  }
}
```

### Access Protected Routes

Use the token in the `Authorization` header:
```bash
curl http://localhost:8080/api/auth/me \
  -H "Authorization: Bearer YOUR_TOKEN_HERE"
```

### Password Requirements

- Minimum 8 characters
- At least one letter
- At least one number

## 🔒 Security Features

- ✅ **Password Hashing**: Bcrypt with salt
- ✅ **JWT Tokens**: HS256 algorithm
- ✅ **Token Expiration**: 24 hours
- ✅ **Password Validation**: Strength requirements
- ✅ **Protected Routes**: Bearer token authentication

## 🏗️ Project Structure
```
cpp-backend-services/
├── src/
│   ├── main.cpp                  # Entry point
│   ├── handlers/                 # Request handlers
│   │   ├── UserHandlers.cpp
│   │   └── HealthHandler.cpp
│   ├── models/                   # Data models
│   │   ├── User.h
│   │   └── ApiResponse.h
│   ├── repositories/             # Database access layer
│   │   └── UserRepository.h
│   ├── database/                 # Database management
│   │   ├── DatabaseManager.h
│   │   └── DatabaseManager.cpp
│   ├── exceptions/               # Custom exceptions
│   │   └── ApiException.h
│   ├── utils/                    # Utilities
│   │   ├── RequestLogger.h
│   │   └── ErrorHandler.h
│   └── middleware/               # Middleware (optional)
├── build/                        # Build output (not in git)
├── logs/                         # Log files (not in git)
├── CMakeLists.txt               # CMake configuration
├── start-server.sh              # Startup script
├── .gitignore                   # Git ignore rules
└── README.md                    # This file
```

## 🔒 Configuration

Database credentials are currently hardcoded. For production:

1. Move credentials to environment variables
2. Use a `.env` file (add to .gitignore)
3. Never commit sensitive data

## 🧪 Testing
```bash
# Test all endpoints
curl http://localhost:8080/health
curl http://localhost:8080/health/database
curl http://localhost:8080/api/users
curl http://localhost:8080/api/users/1
```

## 🚧 Roadmap

- [x] RESTful API endpoints
- [x] PostgreSQL database integration
- [x] JWT Authentication
- [x] User registration & login
- [x] Password hashing (bcrypt)
- [x] Protected routes
- [x] Request logging with timing
- [x] Global error handling
- [x] Data validation
- [x] Health check endpoints
- [x] **Docker containerization**
- [x] **Docker Compose orchestration**



## ⚠️ Security Notice

**For Production Deployment:**

1. **Change JWT Secret**: Update the secret key in `src/utils/JwtToken.h` or use environment variables
2. **Use HTTPS**: Always use TLS/SSL in production
3. **Environment Variables**: Move credentials from code to `.env` file
4. **Rate Limiting**: Implement rate limiting on authentication endpoints
5. **Input Sanitization**: Additional validation for production use
6. **CORS Configuration**: Configure CORS properly for your frontend domain
7. **Database Passwords**: Use strong, unique passwords
8. **Keep Dependencies Updated**: Regular security updates

**Current implementation uses hardcoded credentials for development only!**
```

---

## 📊 PHASE 5 COMPLETE SUMMARY
```
✅ Phase 1: Environment Setup (15%) - COMPLETE
✅ Phase 2: Core Backend (15%) - COMPLETE
✅ Phase 3: Professional Architecture (15%) - COMPLETE
✅ Phase 4: Database Integration (15%) - COMPLETE
✅ Phase 5: JWT Authentication (15%) - COMPLETE

Total Progress: 75% COMPLETE! 🎉

## 🙏 Acknowledgments

- [Drogon Framework](https://github.com/drogonframework/drogon)
- [libpqxx](https://github.com/jtv/libpqxx)

## 🐳 Docker Deployment (Recommended)

The easiest way to run this application is with Docker:
```bash
# Build and start
docker-compose up -d

# Check status
docker-compose ps

# View logs
docker-compose logs -f app

# Test
curl http://localhost:8080/health
```

**See [DOCKER.md](DOCKER.md) for complete Docker documentation.**

---