@echo off
cls
echo Attempting compile...
echo.
Call build.bat
IF %ERRORLEVEL% EQU 0 (
    echo.
    echo Compile success!  Running...
    Call test.bat > test.out
    echo.
    echo Output:
    echo.
    type test.out
    echo.
    ) ELSE (
        echo.
        echo Compile failed!
    )
    