@echo off
echo ========================================
echo Building Nexus-Drv-TestDemo
echo ========================================

REM 设置 Visual Studio 环境
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

REM 编译项目
msbuild "Nexus-Drv-TestDemo.vcxproj" /p:Configuration=Release /p:Platform=x64 /t:Rebuild

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Build succeeded!
    echo ========================================
    echo.
    echo Output: x64\Release\Nexus-Drv-TestDemo.exe
) else (
    echo.
    echo ========================================
    echo Build failed!
    echo ========================================
)

pause
