# Exit on error
$ErrorActionPreference = "Stop"

# Check if the build output directory exists
if (-Not (Test-Path "build/bin/everest-editor")) {
    Write-Host "Running CMake configuration..."
    cmake -B build -DBuild=Release -DTarget=Editor
}

# Try building the project
Write-Host "Building project..."
if (cmake --build build/) {
    Write-Host "----------- Build Successful ------------"
} else {
    Write-Host "-x-x-x-x-x-x Build failed x-x-x-x-x-x-"
}

