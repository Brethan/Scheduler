#!/bin/bash

make
./main.exe ./data/test_case_1.csv ./outputTest1.csv
make visual
rm ./*.o
./mainV.exe 800000 200000 ./outputTest1.csv

