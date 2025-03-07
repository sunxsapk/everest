#!/bin/bash

# Exit immediately if any command fails
set -e

# echo "Updating package list..."
# sudo apt update -y

echo "Installing CMake..."
sudo apt install -y cmake

echo "Installing C++ compiler (GCC/G++)..."
sudo apt install -y build-essential

echo "Installing GLFW..."
sudo apt install -y libglfw3-dev

echo "Installing Lua..."
sudo apt install -y lua5.4

echo "Installing Zenity..."
sudo apt install -y zenity

echo "Verifying installations..."

# Check CMake
if command -v cmake &> /dev/null; then
    echo "✅ CMake installed: $(cmake --version | head -n 1)"
else
    echo "❌ CMake installation failed!"
fi

# Check C++ Compiler
if command -v g++ &> /dev/null; then
    echo "✅ C++ Compiler installed: $(g++ --version | head -n 1)"
else
    echo "❌ C++ Compiler installation failed!"
fi

# Check GLFW
if dpkg -l | grep -q "libglfw3-dev"; then
    echo "✅ GLFW installed!"
else
    echo "❌ GLFW installation failed!"
fi

# Check Lua
if command -v lua &> /dev/null; then
    echo "✅ Lua installed: $(lua -v)"
else
    echo "❌ Lua installation failed!"
fi

# Check Zenity
if command -v zenity &> /dev/null; then
    echo "✅ Zenity installed!"
else
    echo "❌ Zenity installation failed!"
fi

echo "🎉 All dependencies installed successfully!"

