#!/bin/bash

make
./main.exe ./data/test_case_2.csv ./outputTest2.csv

make visual
rm ./*.o
./mainV.exe 800000 200000 ./outputTest2.csv

