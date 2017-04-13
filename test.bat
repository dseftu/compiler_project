@echo off
cls
echo Compiler for COP 3402 - Systems Software
echo Authored by Dallas Seroski and David M. Allen
echo Spring 2017
echo =============================================
echo nested Program (sample.pl0)
echo.
echo Tests a basic program.
echo.
type .\GoodPL0Files\sample.pl0
echo.
echo.
echo .\bin\compiler.exe .\GoodPL0Files\sample.pl0
echo.
echo.
echo. Expected output: 59, 3, 4, 63
echo.
.\bin\compiler.exe -a -v -l .\GoodPL0Files\sample.pl0
echo =============================================
echo test2 Program (test2.pl0)
echo.
echo Tests recursion.  Answer should be 3!.
echo.
type .\GoodPL0Files\test2.pl0
echo.
echo.
echo .\bin\compiler.exe .\GoodPL0Files\test2.pl0
echo.
echo.
echo. Expected output: 6
echo.
.\bin\compiler.exe .\GoodPL0Files\test2.pl0
echo =============================================
echo test3 Program (test3.pl0)
echo.
echo Tests procedures.
echo.
type .\GoodPL0Files\test3.pl0
echo.
echo.
echo .\bin\compiler.exe .\GoodPL0Files\test3.pl0
echo.
echo.
echo. Expected output: 5, 2, 10, 5
echo.
.\bin\compiler.exe .\GoodPL0Files\test3.pl0
echo =============================================
echo nested Program (nested.pl0)
echo.
echo Tests functionality of nested procedures.
echo.
echo.
type .\GoodPL0Files\nested.pl0
echo.
echo.
echo .\bin\compiler.exe .\GoodPL0Files\nested.pl0
echo.
echo.
echo. Expected output: 9, 7, 12, 6, 5
echo.
.\bin\compiler.exe .\GoodPL0Files\nested.pl0
echo =============================================
echo nested Program (test.pl0)
echo.
echo Tests functionality of if then else
echo.
echo.
type .\GoodPL0Files\test.pl0
echo.
echo.
echo .\bin\compiler.exe .\GoodPL0Files\test.pl0
echo.
echo Expected input: integer
echo.
echo "Expected output: if input > 4, then print input + 1.  Else print 4.
echo.
.\bin\compiler.exe .\GoodPL0Files\test.pl0
