# CMS-Privacy
On the Privacy of the Count-Min Sketch

## Description
The goal of Count-Min Sketch privacy is to ... This code is used in the paper "On the Privacy of Count-Min Sketch" by Alfonso Sánchez-Macián, Pedro Reviriego, Jorge Martinez and ...

## Compilation
This Visual Studio C++ project consists of 12 files including header files, source files and the main program.

## Command line arguments
Command line arguments are:
- w: table width, default value is w=1048576
- d: table depth or number of counters checked, default value is d=2
- f: input keys
- p: minimum value of P, default value is 256
- i: increment of p, default value is 128
- a: heavy hitter selection 1:first, 2:highest estimate, 3:lowest estimate, default value is 2

**example: icms.exe w=16384 d=4, f=ch1.txt p=256 i=128 a=2**

## License

MIT
