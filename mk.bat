
@REM Compiling...
@REM Note: Might tinker with these options later -mfpmath=sse -msse2

set CXXPRTF=-Wformat -Wno-unused-parameter -Werror=format -Werror=format-extra-args -Werror=format-zero-length -Werror=nonnull -Werror=format-contains-nul
set INCDIR=
set LIBDIR=
set LIBS=
x86_64-w64-mingw32-g++.exe -w -Wall -Wl,--stack,8000000 -static -std=c++11 -O2 %INCDIR% -o test.exe test.cpp %LIBDIR% %LIBS%


