#!/bin/bash

# Compiling each source files
gcc -c libs/core.c -o core.o
gcc -c libs/utils.c -o utils.o
gcc -c main.c -o main.o

# Creating the final executable file
gcc main.o core.o utils.o -o orion

# Removing compiled files
rm utils.o
rm core.o
rm main.o