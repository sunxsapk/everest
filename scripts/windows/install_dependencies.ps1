# Run this script as Administrator!

# Enable script execution if not enabled
Set-ExecutionPolicy Bypass -Scope Process -Force

Write-Host "Checking for Chocolatey..."
if (-Not (Test-Path "C:\ProgramData\chocolatey")) {
    Write-Host "Chocolatey not found. Installing Chocolatey..."
    Set-ExecutionPolicy Bypass -Scope Process -Force;
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072;
    Invoke-Expression ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
} else {
    Write-Host "✅ Chocolatey is already installed."
}

Write-Host "Refreshing environment variables..."
refreshenv

Write-Host "Installing CMake..."
choco install cmake -y

Write-Host "Installing MinGW-w64 (GCC/G++)..."
choco install mingw -y

Write-Host "Installing GLFW..."
choco install glfw -y

Write-Host "Installing Lua..."
choco install lua -y

# Write-Host "Installing Zenity..."
# choco install zenity -y

Write-Host "Verifying installations..."

# Check CMake
if (Get-Command cmake -ErrorAction SilentlyContinue) {
    Write-Host "✅ CMake installed: $(cmake --version)"
} else {
    Write-Host "❌ CMake installation failed!"
}

# Check GCC/G++
if (Get-Command g++ -ErrorAction SilentlyContinue) {
    Write-Host "✅ C++ Compiler installed: $(g++ --version)"
} else {
    Write-Host "❌ C++ Compiler installation failed!"
}

# Check GLFW
if (choco list --localonly | Select-String "glfw") {
    Write-Host "✅ GLFW installed!"
} else {
    Write-Host "❌ GLFW installation failed!"
}

# Check Lua
if (Get-Command lua -ErrorAction SilentlyContinue) {
    Write-Host "✅ Lua installed: $(lua -v)"
} else {
    Write-Host "❌ Lua installation failed!"
}

# Check Zenity
# if (Get-Command zenity -ErrorAction SilentlyContinue) {
#     Write-Host "✅ Zenity installed!"
# } else {
#     Write-Host "❌ Zenity installation failed!"
# }

Write-Host "🎉 All dependencies installed successfully!"

