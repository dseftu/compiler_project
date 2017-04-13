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

gcc -o ./compiler ./src/compiler.c ./src/errorHandler.c ./src/icg.c ./src/P-machine.c ./src/parser.c ./src/scanner.c

Alternatively there is a build.sh script included that can also compile the program on eustis which can be run as follows:

chmod +x ./build.sh
./build.sh

To build from a windows prompt:

build.bat

To run a sample set of programs (the last program requires a single numeric input):

test.bat

There are small sample PL0 files included with this assignment in the ./GoodPL0Files/ folder as well as several
PL0 files with errors that the parser should be able to detect in ./ErrorPL0Files/