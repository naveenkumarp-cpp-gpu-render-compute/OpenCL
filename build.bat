@echo off
cd /d "%~dp0"

set EXE=OpenCLTest.exe


echo =========================
echo Checking source changes
echo =========================


REM Configure only if build folder does not exist
if not exist build (
    echo Build folder missing. Configuring CMake...
    cmake -S . -B build -G "Visual Studio 17 2022"

    if errorlevel 1 exit /b 1
)


REM Check if any source/header file is newer than executable
set NEED_BUILD=0


if not exist "%EXE%" (
    set NEED_BUILD=1
)


for /r %%f in (*.cpp *.h *.cl *.cmake CMakeLists.txt) do (

    if exist "%EXE%" (

        for %%e in ("%EXE%") do (

            if %%~tf GTR %%~te (
                set NEED_BUILD=1
            )

        )

    )

)



if "%NEED_BUILD%"=="0" (

    echo No source changes detected.
    echo Executable is up to date.

    goto COPY

)



echo =========================
echo Building
echo =========================


cmake --build build --config Release


if errorlevel 1 exit /b 1



:COPY

echo =========================
echo Copy executable
echo =========================


copy /y "build\Release\OpenCLTest.exe" "OpenCLTest.exe"


if errorlevel 1 exit /b 1



echo =========================
echo Build complete
echo =========================

echo Run manually:
echo OpenCLTest.exe
