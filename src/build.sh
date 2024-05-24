#!/bin/bash

# Compiling each source files
gcc -c libs/core.c -o core.o
gcc -c libs/fileio.c -o fileio.o
gcc -c libs/dirio.c -o dirio.o
gcc -c main.c -o main.o

# Creating the final executable file
gcc main.o core.o fileio.o dirio.o -o orion

# Removing compiled files
rm dirio.o
rm fileio.o
rm core.o
rm main.o