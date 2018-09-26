..\deps\win32\bin\win_bison -v -d -l -o fay_compiler_parser.cpp fay.y
..\deps\win32\bin\win_flex -o fay_compiler_tokens.cpp fay.l
rem "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Common7\IDE\devenv.exe" ../build/fay.sln /build Debug /project fay
rem ..\build\bin\Debug\fay.exe
