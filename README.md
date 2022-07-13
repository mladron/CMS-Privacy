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

## Running experiments

The experiments were run with a batch file. Traces were downloaded from CAIDA

https://www.caida.org/catalog/datasets/passive_dataset_download

// San José

xxx
xxx
xxx

icms f=sj12.txt d=3 w=4096 p=128 i=64 s=45687
icms f=sj12.txt d=4 w=4096 p=128 i=64 s=45687
icms f=sj12.txt d=5 w=4096 p=128 i=64 s=45687

icms f=sj12.txt d=3 w=8192 p=128 i=64 s=45687
icms f=sj12.txt d=4 w=8192 p=128 i=64 s=45687
icms f=sj12.txt d=5 w=8192 p=128 i=64 s=45687

icms f=sj12.txt d=3 p=128 i=64 s=45687
icms f=sj12.txt d=4 p=128 i=64 s=45687
icms f=sj12.txt d=5 p=128 i=64 s=45687

// Chicago A

icms f=ch1.txt d=3 w=4096 p=128 i=64 s=45687
icms f=ch1.txt d=4 w=4096 p=128 i=64 s=45687
icms f=ch1.txt d=5 w=4096 p=128 i=64 s=45687

icms f=ch1.txt d=3 w=8192 p=128 i=64 s=45687
icms f=ch1.txt d=4 w=8192 p=128 i=64 s=45687
icms f=ch1.txt d=5 w=8192 p=128 i=64 s=45687

icms f=ch1.txt d=3 p=128 i=64 s=45687
icms f=ch1.txt d=4 p=128 i=64 s=45687
icms f=ch1.txt d=5 p=128 i=64 s=45687

// Chicago B

icms f=ch2.txt d=3 w=4096 p=128 i=64 s=45687
icms f=ch2.txt d=4 w=4096 p=128 i=64 s=45687
icms f=ch2.txt d=5 w=4096 p=128 i=64 s=45687

icms f=ch2.txt d=3 w=8192 p=128 i=64 s=45687
icms f=ch2.txt d=4 w=8192 p=128 i=64 s=45687
icms f=ch2.txt d=5 w=8192 p=128 i=64 s=45687

icms f=ch2.txt d=3 p=128 i=64 s=45687
icms f=ch2.txt d=4 p=128 i=64 s=45687
icms f=ch2.txt d=5 p=128 i=64 s=45687


## License

MIT
