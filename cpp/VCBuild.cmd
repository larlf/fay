%~d0
cd %~dp0
mkdir build
cd build
cmake -G "Visual Studio 15 Win64" ..
rem cmake -G "Visual Studio 15" ..
mkdir bin
mkdir bin\Debug
copy /y ..\deps\win32\iconv\lib\*.dll bin\Debug
copy /y ..\deps\win32\jasson\lib\*.dll bin\Debug
copy /y ..\deps\win32\mysql\lib\*.dll bin\Debug
copy /y ..\deps\win32\pthread\lib\*.dll bin\Debug
copy /y ..\deps\win32\uv\lib\*.dll bin\Debug
copy /y ..\deps\win32\zlib\lib\*.dll bin\Debug
copy /y ..\deps\win32\mysql\lib\*.dll bin\Debug
copy /y ..\deps\win32\mysql\mirage\*.dll bin\Debug
copy /y ..\deps\win32\vld\lib\*.dll bin\Debug
copy /y ..\deps\win32\vld\lib\*.pdb bin\Debug
copy /y ..\deps\win32\vld\lib\*.manifest bin\Debug
cd ..
pause