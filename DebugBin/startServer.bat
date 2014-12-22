@echo off
echo start server ...

start NDLoginServer.exe 1 &
@ping localhost -n 3 >nul
start NDWorldServer.exe 1 &
@ping localhost -n 3 >nul