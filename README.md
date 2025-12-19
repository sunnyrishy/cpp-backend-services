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

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/` | API information |
| GET | `/health` | Basic health check |
| GET | `/health/database` | Database health check |
| GET | `/api/users` | Get all users |
| GET | `/api/users/{id}` | Get user by ID |
| POST | `/api/users` | Create new user |

## 📝 Example API Calls

### Get All Users
```bash
curl http://localhost:8080/api/users
```

**Response:**
```json
{
  "status": "success",
  "message": "Users retrieved successfully from database",
  "count": 3,
  "data": [
    {
      "id": "1",
      "name": "John Doe",
      "email": "john@example.com",
      "createdAt": "2024-12-19T10:00:00Z"
    }
  ]
}
```

### Create User
```bash
curl -X POST http://localhost:8080/api/users \
  -H "Content-Type: application/json" \
  -d '{"name":"Bob Wilson","email":"bob@example.com"}'
```

**Response:**
```json
{
  "status": "success",
  "message": "User created successfully in database",
  "data": {
    "id": "4",
    "name": "Bob Wilson",
    "email": "bob@example.com",
    "createdAt": "2024-12-19T10:30:00Z"
  }
}
```

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

- [ ] JWT Authentication
- [ ] User login/register endpoints
- [ ] Password hashing
- [ ] Pagination
- [ ] Filtering & sorting
- [ ] Unit tests
- [ ] Docker containerization
- [ ] CI/CD pipeline


## 🙏 Acknowledgments

- [Drogon Framework](https://github.com/drogonframework/drogon)
- [libpqxx](https://github.com/jtv/libpqxx)