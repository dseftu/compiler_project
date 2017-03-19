@echo off
cls
gcc -o compiler.exe compiler.c errorHandler.c icg.c P-machine.c parser.c scanner.c
pause
cls
compiler.exe testprogram2.pl0