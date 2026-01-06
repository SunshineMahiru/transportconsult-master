@echo off
title 交通咨询系统编译运行工具
color 0A

echo =================================
echo     交通咨询系统编译运行工具
echo =================================
echo.

:CHECK_EXE
if exist "transport_system.exe" (
    goto EXE_EXISTS
) else (
    goto EXE_NOT_EXISTS
)

:EXE_EXISTS
echo 检测到已存在的可执行文件: transport_system.exe
echo.
echo 请选择操作:
echo   [1] 直接运行现有程序
echo   [2] 删除现有文件并重新编译
echo   [3] 重新编译(覆盖现有文件)
echo   [4] 只编译不运行
echo   [5] 退出
echo.
set /p choice=请输入选择(1-5): 

if "%choice%"=="1" goto RUN_EXISTING
if "%choice%"=="2" goto DELETE_AND_COMPILE
if "%choice%"=="3" goto COMPILE_OVERWRITE
if "%choice%"=="4" goto COMPILE_ONLY
if "%choice%"=="5" goto EXIT

echo 无效选择，请重新输入
echo.
goto EXE_EXISTS

:EXE_NOT_EXISTS
echo 未找到已编译的可执行文件
echo.
echo 请选择操作:
echo   [1] 编译并运行
echo   [2] 只编译不运行
echo   [3] 退出
echo.
set /p choice=请输入选择(1-3): 

if "%choice%"=="1" goto COMPILE_AND_RUN
if "%choice%"=="2" goto COMPILE_ONLY
if "%choice%"=="3" goto EXIT

echo 无效选择，请重新输入
echo.
goto EXE_NOT_EXISTS

:DELETE_AND_COMPILE
echo.
echo 正在删除现有文件...
del transport_system.exe
if exist "transport_system.exe" (
    echo 无法删除文件，请手动检查
    pause
    goto EXIT
)
echo 删除成功！
goto COMPILE_AND_RUN

:COMPILE_OVERWRITE
echo.
echo 将覆盖现有文件进行编译...
set RUN_AFTER_COMPILE=1
goto COMPILE

:COMPILE_ONLY
echo.
echo 开始编译(不运行)...
set RUN_AFTER_COMPILE=0
goto COMPILE

:RUN_EXISTING
echo.
echo =================================
echo     运行现有程序
echo =================================
echo.
transport_system.exe
echo.
pause
goto EXIT

:COMPILE_AND_RUN
echo.
set RUN_AFTER_COMPILE=1
goto COMPILE

:COMPILE
echo =================================
echo     编译交通咨询系统
echo =================================
echo.

REM 检查源文件是否存在
echo 检查源文件...
set MISSING_FILES=0
if not exist "graph.cpp" (
    echo [错误] 找不到 graph.cpp
    set MISSING_FILES=1
)
if not exist "algorithm.cpp" (
    echo [错误] 找不到 algorithm.cpp
    set MISSING_FILES=1
)
if not exist "main.cpp" (
    echo [错误] 找不到 main.cpp
    set MISSING_FILES=1
)

if %MISSING_FILES%==1 (
    echo.
    echo 请确保以下文件在当前目录:
    echo   - graph.cpp
    echo   - algorithm.cpp
    echo   - main.cpp
    echo.
    pause
    goto EXIT
)

echo [OK] 所有源文件都存在
echo.

REM 设置编译器路径
set COMPILER=g++

echo 编译参数: %COMPILER% -std=c++17 graph.cpp algorithm.cpp main.cpp -o transport_system.exe
echo.

REM 编译命令
echo 开始编译...
%COMPILER% -std=c++17 graph.cpp algorithm.cpp main.cpp -o transport_system.exe

REM 检查编译是否成功
if %errorlevel% equ 0 (
    echo [成功] 编译成功！
    
    if "%RUN_AFTER_COMPILE%"=="1" (
        echo.
        echo =================================
        echo     运行交通咨询系统
        echo =================================
        echo.
        transport_system.exe
    ) else (
        echo [完成] 编译完成，可执行文件已生成
    )
) else (
    echo [失败] 编译失败！
    echo 错误代码: %errorlevel%
    echo 请检查源代码中的错误
)

echo.
if "%RUN_AFTER_COMPILE%"=="1" (
    echo 程序执行完毕
) else (
    echo 操作完成
)

:EXIT
echo.
echo 按任意键退出...
pause > nul
exit /b %errorlevel%