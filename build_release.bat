@echo off
REM ============================================================
REM  One-click build: MinGW-w64 (UCRT64) fully static Release
REM  Output: output\FolkFight.exe (single file, no third-party DLLs)
REM
REM  Set MINGW below to your MinGW-w64 bin folder.
REM ============================================================
setlocal

set "ROOT=%~dp0"
set "MINGW=D:\vscode\mingw64\bin"
set "LIB=%ROOT%.dl\mingwlibs\ucrt64\lib"
set "INC=%ROOT%.dl\mingwlibs\ucrt64\include"
set "CRTINC=%ROOT%toolchain\gccroot\x86_64-w64-mingw32\include"
set "OUT=%ROOT%output\FolkFight.exe"
set "PATH=%ROOT%toolchain\bin;%MINGW%;%PATH%"

if not exist "%MINGW%\g++.exe" (
  echo [ERROR] compiler not found: %MINGW%\g++.exe
  echo Edit the MINGW variable at the top of this script.
  exit /b 1
)

if not exist "%ROOT%output" mkdir "%ROOT%output"

echo Building (fully static link, about 1 minute)...

cd /d "%LIB%"

"%MINGW%\g++.exe" -std=c++20 -O2 -fpermissive -w ^
  -DSFML_STATIC -static -static-libgcc -static-libstdc++ ^
  -o "%OUT%" ^
  "%ROOT%main.cpp" "%ROOT%susake.cpp" ^
  -idirafter "%CRTINC%" ^
  -I"%INC%" ^
  "libsfml-graphics-s.a" "libsfml-window-s.a" "libsfml-audio-s.a" "libsfml-system-s.a" ^
  "libbox2d.a" ^
  "libfreetype.a" "libharfbuzz.a" "libgraphite2.a" "libpng16.a" "libz.a" "libbz2.a" ^
  "libbrotlidec.a" "libbrotlicommon.a" ^
  "libFLAC.a" "libvorbisfile.a" "libvorbisenc.a" "libvorbis.a" "libogg.a" ^
  "libopenal.a" ^
  -lopengl32 -lgdi32 -lwinmm -lole32 -lshell32 -luser32 -ladvapi32 ^
  -lws2_32 -lwinhttp -ldwrite -luuid -lavrt -lshlwapi -lversion ^
  -lwininet -lcfgmgr32 -lrpcrt4

if errorlevel 1 (
  echo.
  echo [FAILED] build error, see output above.
  exit /b 1
)

echo.
echo [OK] built: %OUT%
echo Put the resources folder next to the exe when running.
exit /b 0
