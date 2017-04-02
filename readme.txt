# compiler_project
Compiler for COP 3402 - Systems Software
Authored by Dallas Seroski and David M. Allen
Compiler Project for COP 3402 Systems Software

Command Line Arguments Description:

Expected:  compiler [-l] [-a] [-v] inputfile

inputfile       The PL/0 file to be compiled as specified in the assignment documentation
-l      Prints the list of lexemes
-a      Prints the object code
-v      Prints the VM execution trace

Compilation instructions:

from the eustis server prompt:

gcc -o ./bin/compiler ./src/compiler.c ./src/errorHandler.c ./src/icg.c ./src/P-machine.c ./src/parser.c ./src/scanner.c

There is a small sample PL0 file included with this assignment in the ./GoodPL0Files/ folder as well as several
PL0 files with errors that the parser should be able to detect in ./ErrorPL0Files/