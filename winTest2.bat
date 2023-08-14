@ECHO OFF
gcc .\src\main.c .\src\pcb_queue.c .\src\io_process.c -o simulator.exe
simulator.exe .\data\test_case_2.csv .\outputTest2.csv

.\outputTest2.csv
PAUSE
