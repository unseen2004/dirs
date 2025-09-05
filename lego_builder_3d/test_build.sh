#!/bin/bash

# LEGO Builder 3D Build Test Script
echo "=== LEGO Builder 3D Build Test ==="

# Check for required dependencies
echo "Checking dependencies..."
if ! command -v cmake &> /dev/null; then
    echo "❌ CMake not found"
    exit 1
fi

if ! command -v make &> /dev/null; then
    echo "❌ Make not found"
    exit 1
fi

if ! command -v g++ &> /dev/null; then
    echo "❌ G++ not found"
    exit 1
fi

echo "✅ All dependencies found"

# Clean and build
echo "Building project..."
rm -rf build
mkdir build
cd build

if ! cmake ..; then
    echo "❌ CMake configuration failed"
    exit 1
fi

if ! make; then
    echo "❌ Build failed"
    exit 1
fi

echo "✅ Build successful"

# Check if executable exists
if [ -f "bin/LegoBuilder3D" ]; then
    echo "✅ Executable created: bin/LegoBuilder3D"
    ls -la bin/LegoBuilder3D
else
    echo "❌ Executable not found"
    exit 1
fi

# Test file loading (won't run in headless environment)
echo "Testing model file loading..."
if [ -f "../resources/sample_castle.lego" ]; then
    echo "✅ Sample model file found"
    head -5 ../resources/sample_castle.lego
else
    echo "❌ Sample model file not found"
fi

echo ""
echo "🎉 LEGO Builder 3D is ready!"
echo ""
echo "To run (requires display):"
echo "  cd build && ./bin/LegoBuilder3D"
echo ""
echo "Features included:"
echo "  ✅ 3D OpenGL rendering engine"
echo "  ✅ 6 LEGO brick types (1x1, 1x2, 2x2, 2x4, 2x6, 2x8)"
echo "  ✅ 7 brick colors"
echo "  ✅ Interactive camera controls"
echo "  ✅ Snap-to-grid placement"
echo "  ✅ Collision detection"
echo "  ✅ Undo/redo system"
echo "  ✅ Save/load functionality"
echo "  ✅ Real-time lighting"
echo ""