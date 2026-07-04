@echo off
echo Setting up MSVC environment...
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
if errorlevel 1 exit /b %errorlevel%

echo Building debug x64...
cl.exe /EHsc /std:c++latest /MDd /Zi /Fe:output\myapp_debug_x64.exe /I"C:/Users/12625/Desktop/SFML/include" /I"D:/box/box2d-3.1.1/include" main.cpp susake.cpp /link /LIBPATH:"C:/Users/12625/Desktop/SFML/lib" /LIBPATH:"D:/box/box2d-3.1.1/build/src/Debug" /MACHINE:X64 box2dd.lib sfml-window-d.lib sfml-system-d.lib sfml-graphics-d.lib sfml-audio-d.lib
if errorlevel 1 exit /b %errorlevel%

echo Build successful.