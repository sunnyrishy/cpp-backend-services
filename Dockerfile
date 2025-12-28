# Use Ubuntu 24.04 as base
FROM ubuntu:24.04

# Avoid prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install ALL dependencies (runtime + Drogon build deps)
RUN apt-get update && apt-get install -y \
    libssl3 \
    libpq5 \
    libpqxx-7.8t64 \
    libjsoncpp25 \
    libjsoncpp-dev \
    libc-ares2 \
    libc-ares-dev \
    libbrotli1 \
    libbrotli-dev \
    zlib1g \
    zlib1g-dev \
    uuid-runtime \
    uuid-dev \
    curl \
    git \
    cmake \
    build-essential \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Drogon framework
RUN git clone https://github.com/drogonframework/drogon.git /tmp/drogon && \
    cd /tmp/drogon && \
    git submodule update --init && \
    mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make install && \
    ldconfig && \
    rm -rf /tmp/drogon

# Create app directory
WORKDIR /app

# Copy pre-built binary from local build
COPY build/cpp-backend-services /app/cpp-backend-services

# Make it executable
RUN chmod +x /app/cpp-backend-services

# Create logs directory
RUN mkdir -p /app/logs

# Expose port
EXPOSE 8080

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=40s --retries=3 \
    CMD curl -f http://localhost:8080/health || exit 1

# Run the application
CMD ["./cpp-backend-services"]