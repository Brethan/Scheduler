#!/bin/bash

python3 randomizer.py

make
./main.exe ./data/test_case_ran.csv ./outputTestRan.csv

make visual
rm ./*.o
./mainV.exe 800000 150000 ./outputTestRan.csv


