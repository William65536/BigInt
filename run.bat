@echo off

gcc -g -c src\main.c -o obj\main.o

gcc obj\main.o obj\bigint.o -o bin\main

bin\main.exe