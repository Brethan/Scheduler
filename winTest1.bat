@ECHO OFF
gcc .\src\main.c .\src\pcb_queue.c .\src\io_process.c -o simulator.exe
simulator.exe .\data\test_case_1.csv .\outputTest1.csv

.\outputTest1.csv
PAUSE
