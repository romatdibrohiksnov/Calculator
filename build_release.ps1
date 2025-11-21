# Build Release Script for Professional Calculator

# 1. Create build directory
if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Force -Path "build" | Out-Null
    Write-Host "Created build directory." -ForegroundColor Green
}

# 2. Configure CMake
Write-Host "Configuring CMake..." -ForegroundColor Cyan
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed."
    exit 1
}

# 3. Build Release
Write-Host "Building Release..." -ForegroundColor Cyan
cmake --build . --config Release
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed."
    exit 1
}

# 4. Locate Executable
$exePath = ".\Release\ProfessionalCalculator.exe"
if (Test-Path $exePath) {
    Write-Host "Build Success!" -ForegroundColor Green
    Write-Host "Executable located at: $(Resolve-Path $exePath)" -ForegroundColor Yellow
    
    # Optional: Open folder
    # Invoke-Item ".\Release"
} else {
    Write-Warning "Build finished but executable not found at expected path: $exePath"
}

cd ..
Write-Host "Done."
