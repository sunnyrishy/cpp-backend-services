#!/bin/bash

echo "=========================================="
echo "🚀 Starting C++ Backend Server"
echo "=========================================="
echo ""

# Start PostgreSQL if not running
echo "🔧 Checking PostgreSQL status..."
if ! sudo service postgresql status > /dev/null 2>&1; then
    echo "📦 Starting PostgreSQL..."
    sudo service postgresql start
else
    echo "✅ PostgreSQL already running"
fi

echo ""
echo "🏗️  Building project..."
cd build

# Build the project
if make -j4; then
    echo ""
    echo "✅ Build successful!"
    echo ""
    echo "🚀 Starting server..."
    echo ""
    ./cpp-backend-services
else
    echo ""
    echo "❌ Build failed!"
    exit 1
fi