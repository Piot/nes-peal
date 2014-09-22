@echo off
cc65\bin\cl65.exe -t nes -I cc65\include -L cc65\lib main.c -o peal.nes
ping 127.0.0.1 -n 2
start peal.nes
