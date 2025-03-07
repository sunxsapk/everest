#!/bin/bash

# Exit immediately if any command fails
set -e

echo "Checking for Homebrew..."
if ! command -v brew &> /dev/null; then
    echo "Homebrew not found. Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
else
    echo "✅ Homebrew is already installed."
fi

echo "Updating Homebrew..."
brew update

echo "Installing CMake..."
brew install cmake

echo "Installing C++ compiler (GCC/G++)..."
xcode-select --install || echo "✅ Xcode command line tools already installed."

echo "Installing GLFW..."
brew install glfw

echo "Installing Lua..."
brew install lua

echo "Installing Zenity..."
brew install zenity

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
if brew list | grep -q "glfw"; then
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

