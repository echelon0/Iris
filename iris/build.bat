@echo off

pushd .
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
REM call "A:\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
popd

pushd ..\build
del *.pdb 1> NUL 2> NUL

set compiler_flags=-nologo -W4 -WX -wd4127 -wd4996 -wd4100 -wd4189 -wd4100 -wd4505 -wd4706 /Wv:18 -nologo -Z7 /EHsc -Od
set linker_flags=-incremental:no User32.lib Gdi32.lib D3DCompiler.lib D3D11.lib DSound.lib winmm.lib

cl %compiler_flags% ..\iris\win32_main.cpp /link %linker_flags%

popd
