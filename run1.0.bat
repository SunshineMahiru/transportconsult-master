
@echo off
echo =================================
echo 编译交通咨询系统
echo =================================

REM 设置编译器路径（根据实际情况调整）
set COMPILER=g++

REM 编译命令
%COMPILER% -std=c++17 graph.cpp algorithm.cpp main.cpp -o transport_system.exe

REM 检查编译是否成功
if %errorlevel% equ 0 (
    echo 编译成功！
    echo.
    echo =================================
    echo 运行交通咨询系统
    echo =================================
    echo.
    transport_system.exe
) else (
    echo 编译失败！
    pause
    exit /b %errorlevel%
)

echo.
pause

```
