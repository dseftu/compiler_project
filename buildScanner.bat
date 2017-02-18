@ECHO OFF
cls
del scanner
bash -c "gcc scanner.c -o scanner"
bash -c "./scanner testprogram.pl0"