%~d0
cd %~dp0
mkdir build
cd build
cmake -G "Visual Studio 15 Win64" ..
rem cmake -G "Visual Studio 15" ..
mkdir bin
mkdir bin\Debug
copy /y ..\deps\win64\iconv\lib\*.dll bin\Debug
copy /y ..\deps\win64\jasson\lib\*.dll bin\Debug
copy /y ..\deps\win64\vld\lib\*.* bin\Debug
cd ..
pause