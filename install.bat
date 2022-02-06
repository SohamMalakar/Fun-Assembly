@echo off

@REM Compile the project
@echo Compiling...
g++ -Ofast -std=c++17 -o fasml main.cpp

@REM Copy the executable to the bin folder
@echo Copying...
copy /Y fasml.exe "C:\Windows"

@REM Clean up
@echo Cleaning up...
del fasml.exe

pause