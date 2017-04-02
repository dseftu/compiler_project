@echo off
cls
gcc -o ./bin/compiler.exe ./src/compiler.c ./src/errorHandler.c ./src/icg.c ./src/P-machine.c ./src/parser.c ./src/scanner.c