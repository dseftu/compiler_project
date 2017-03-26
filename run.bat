@echo off
cls
echo Compiler for COP 3402 - Systems Software
echo Authored by Dallas Seroski and David M. Allen
echo Spring 2017
echo =============================================
echo Sample Program
echo.
echo.
echo const g = 3;
echo var x, y;
echo begin
echo	y := 3;
echo	x := y + 56;
echo end.
echo.
echo compiler.exe -l -a -v testProgram2.pl0
echo.
compiler.exe -l -a -v .\GoodPL0Files\sample.pl0