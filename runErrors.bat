@echo off
cls
echo Compiler for COP 3402 - Systems Software
echo Authored by Dallas Seroski and David M. Allen
echo Spring 2017
echo =============================================
echo Error Examples
echo *******************************************
echo Missing Period:
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error1.pl0
echo *******************************************
echo Incorrect const assignment symbol:
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error2.pl0
echo *******************************************
echo Missing const assignment:
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error3.pl0
echo *******************************************
echo Missing const identifier:
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error4.pl0
echo *******************************************
echo Missing semicolon:
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error5.pl0
echo *******************************************
echo Missing operator:
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error7.pl0
echo *******************************************
echo Missing end statement:
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error8.pl0
echo *******************************************
echo Missing then after if:
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error10.pl0
echo *******************************************
echo Missing do
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error11.pl0
echo *******************************************
echo Undeclared ident
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error12.pl0
echo *******************************************
echo Missing relational operator
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error13.pl0
echo *******************************************
echo Missing right paren
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error14.pl0
echo *******************************************
echo Invalid start of factor
echo.
.\bin\compiler.exe -l -a -v .\ErrorPL0Files\error15.pl0
