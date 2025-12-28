# Docker Deployment Guide

This guide explains how to run the C++ Backend Services using Docker.

## 📋 Prerequisites

- Docker installed
- Docker Compose installed

## 🚀 Quick Start

### 1. Build and Start
```bash
# Build the Docker images
docker-compose build

# Start all services
docker-compose up -d

# Check status
docker-compose ps
```

### 2. View Logs
```bash
# View all logs
docker-compose logs

# View app logs only
docker-compose logs app

# View database logs only
docker-compose logs postgres

# Follow logs in real-time
docker-compose logs -f app
```

### 3. Test the Application
```bash
# Health check
curl http://localhost:8080/health

# Database health check
curl http://localhost:8080/health/database

# Register a user
curl -X POST http://localhost:8080/api/auth/register \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Test User",
    "email": "test@example.com",
    "password": "SecurePass123"
  }'

# Login
curl -X POST http://localhost:8080/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{
    "email": "test@example.com",
    "password": "SecurePass123"
  }'
```

## 🛠️ Management Commands

### Stop Services
```bash
docker-compose down
```

### Stop and Remove Volumes (Clean Database)
```bash
docker-compose down -v
```

### Rebuild After Code Changes
```bash
# Rebuild locally first
cd build && make -j4 && cd ..

# Then rebuild Docker
docker-compose build app
docker-compose up -d
```

### Restart Services
```bash
docker-compose restart
```

### View Running Containers
```bash
docker ps
```

### Execute Commands in Container
```bash
# Access app container
docker exec -it cpp-backend-app bash

# Access database
docker exec -it cpp-backend-db psql -U cpp_backend_user -d cpp_backend_db
```

## 🔧 Configuration

### Environment Variables

The application reads these environment variables:

- `DB_HOST` - Database host (default: postgres)
- `DB_PORT` - Database port (default: 5432)
- `DB_NAME` - Database name (default: cpp_backend_db)
- `DB_USER` - Database user (default: cpp_backend_user)
- `DB_PASSWORD` - Database password

Edit `docker-compose.yml` to change these values.

### Ports

- **Application**: `8080` (host) → `8080` (container)
- **PostgreSQL**: `5433` (host) → `5432` (container)

**Note:** PostgreSQL uses port 5433 on the host to avoid conflicts with local PostgreSQL.

## 📊 Services

### Application (cpp-backend-app)

- **Image**: Built from Dockerfile
- **Port**: 8080
- **Depends on**: postgres
- **Restart Policy**: unless-stopped

### Database (cpp-backend-db)

- **Image**: postgres:16-alpine
- **Port**: 5433 (external) → 5432 (internal)
- **Volume**: postgres_data (persistent storage)
- **Init Script**: init.sql (creates tables and sample data)

## 🔍 Troubleshooting

### Container Won't Start
```bash
# Check logs
docker-compose logs app

# Restart container
docker-compose restart app
```

### Database Connection Issues
```bash
# Check if database is healthy
docker-compose ps

# Verify connection from app container
docker exec -it cpp-backend-app curl http://localhost:8080/health/database
```

### Port Already in Use

If you get "port already in use" error:

1. Check what's using the port: `sudo lsof -i :8080`
2. Stop the conflicting service
3. Or change the port in `docker-compose.yml`

### Clean Rebuild
```bash
# Stop everything
docker-compose down -v

# Remove all containers and images
docker system prune -af

# Rebuild from scratch
docker-compose build --no-cache
docker-compose up -d
```

## 📦 Production Considerations

**For production deployment:**

1. **Use secrets management** for passwords (Docker Secrets, Kubernetes Secrets)
2. **Enable SSL/TLS** for encrypted connections
3. **Add reverse proxy** (nginx, traefik) for HTTPS
4. **Set resource limits** in docker-compose.yml
5. **Configure backup** for PostgreSQL volume
6. **Use tagged images** instead of latest
7. **Enable monitoring** (Prometheus, Grafana)
8. **Add log aggregation** (ELK, Loki)

## 🌐 Deployment Options

### Local Development
```bash
docker-compose up -d
```

### Production (with custom config)
```bash
docker-compose -f docker-compose.yml -f docker-compose.prod.yml up -d
```

### Cloud Deployment
- **AWS**: ECS, EKS, or Fargate
- **Google Cloud**: Cloud Run, GKE
- **Azure**: Container Instances, AKS
- **DigitalOcean**: App Platform, Kubernetes

## 📝 Database Persistence

Data is persisted in the `postgres_data` Docker volume.

### Backup Database
```bash
docker exec cpp-backend-db pg_dump -U cpp_backend_user cpp_backend_db > backup.sql
```

### Restore Database
```bash
cat backup.sql | docker exec -i cpp-backend-db psql -U cpp_backend_user -d cpp_backend_db
```

## ✅ Health Checks

The application includes health check endpoints:

- `/health` - Basic application health
- `/health/database` - Database connectivity

Docker uses these for container health monitoring.