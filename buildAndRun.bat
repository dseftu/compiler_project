@echo off
cls
gcc -o compiler.exe compiler.c errorHandler.c icg.c P-machine.c parser.c scanner.c
cls
compiler.exe testprogram.pl0