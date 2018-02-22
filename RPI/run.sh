#!/bin/bash

rm -r DataBase/

rm -r Semaphore/

mkdir DataBase

mkdir Semaphore

gcc -o Simulator.exe Simulator.c

./Simulator.exe

# rm text.txt

# ls DataBase > text.txt