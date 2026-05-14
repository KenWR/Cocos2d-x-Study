@echo off
setlocal

set "PROJECT_DIR=%~dp0cocos2d-x\projects\PoopDodge"

cd /d "%PROJECT_DIR%" || exit /b 1

cmake -S . -B build-win32 -A Win32 || exit /b 1
cmake --build build-win32 --config Debug || exit /b 1

echo.
echo Build complete:
echo %PROJECT_DIR%\build-win32\bin\PoopDodge\Debug\PoopDodge.exe
