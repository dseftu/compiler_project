@ECHO OFF
cls

bash -c "gcc scanner.c -o scanner.o -c"
bash -c "gcc errorHandler.c -o errorHandler.o -c"
bash -c "gcc compiler.c -o compiler.o -c"
bash -c "gcc icg.c -o icg.o -c"
bash -c "gcc parser.c -o parser.o -c"
bash -c "gcc P-machine.c -o P-machine.o -c"
bash -c "gcc -o compiler compiler.o scanner.o errorHandler.o icg.o parser.o P-machine.o"
bash -c "./compiler"