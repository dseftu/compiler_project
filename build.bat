@echo off
cls
if not exist bin mkdir bin
gcc -o ./bin/compiler ./src/compiler.c ./src/errorHandler.c ./src/icg.c ./src/P-machine.c ./src/parser.c ./src/scanner.c