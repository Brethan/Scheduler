@ECHO OFF

python3 .\randomizer.py

gcc .\src\main.c .\src\pcb_queue.c .\src\io_process.c -o simulator.exe
simulator.exe .\data\test_case_ran.csv .\outputTestRan.csv

.\outputTestRan.csv
PAUSE
