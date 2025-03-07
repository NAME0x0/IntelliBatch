@echo off
REM Build script for Integrated Terminal Desktop (ITD)
TITLE ITD Build Script

echo Integrated Terminal Desktop (ITD) - Build Script
echo ================================================

REM Check for CMake
cmake --version >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
    echo CMake not found. Please install CMake 3.15 or newer.
    pause
    exit /b 1
)

REM Detect Visual Studio
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo Visual Studio not found. Please install Visual Studio 2019 or newer.
    pause
    exit /b 1
)

for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
  set VS_PATH=%%i
)

if "%VS_PATH%" == "" (
    echo Visual Studio not found. Please install Visual Studio 2019 or newer.
    pause
    exit /b 1
)

REM Check for wxWidgets environment variable
if "%WXWIN%" == "" (
    echo WXWIN environment variable not set. Please install wxWidgets and set the WXWIN environment variable.
    pause
    exit /b 1
)

REM Create build directory
if not exist build mkdir build
cd build

REM Configure the project
echo Configuring the project...
cmake -G "Visual Studio 16 2019" -A x64 ..

IF %ERRORLEVEL% NEQ 0 (
    echo Failed to configure the project.
    cd ..
    pause
    exit /b 1
)

REM Build the project
echo Building the project...
cmake --build . --config Release

IF %ERRORLEVEL% NEQ 0 (
    echo Failed to build the project.
    cd ..
    pause
    exit /b 1
)

echo Build completed successfully.
echo The executable is located in build\bin\Release\ITD.exe

cd ..
pause 