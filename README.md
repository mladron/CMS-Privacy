# CMS-Privacy
On the Privacy of the Count-Min Sketch: Extracting the Top-K elements

## Description

Estimating the frequency of elements in a data stream and identifying the elements that appear many times (heavy hitters) is needed in many applications such as traffic monitoring in networks, or popularity estimation in social media. The Count-Min Sketch (CMS) is probably one of the the most widely used algorithms for frequency estimation. This work considers the privacy of the CMS and presents an algorithm to extract the most frequent elements (top-K), and their estimate from a Count-Min sketch. This is possible for universes of a limited size, when the attacker has access to the sketch, its hash functions and the counters at a specific point of time.

This code is used in the paper "On the Privacy of Count-Min Sketch: Extracting the Top-K elements" by Alfonso Sánchez-Macián, Jorge Martínez, Pedro Reviriego, Shanshan Liu and Fabrizio Lombardi, that is in review in IEEE Transactions on Emerging Topics in Computing.

## Compilation
This Visual Studio C++ project consists of 12 files including header files, source files and the main program.

## Command line arguments
Command line arguments are:
- w: table width, default value is w=16384
- d: table depth or number of counters checked, default value is d=2
- f: input keys
- p: minimum value of P, default value is 256
- i: increment of p, default value is 128
- a: heavy hitter selection [1:first, 2:highest estimate, 3:lowest estimate], default value is 2

**example: icms.exe w=16384 d=4, f=ch1.txt p=256 i=128 a=2**

## Input traces and experiments

The traces were downloaded from CAIDA, and the experiments were run with a batch file. Heavy hitters were selected by highest estimate during the peeling process.

https://www.caida.org/catalog/datasets/passive_dataset_download

**San José**

- icms f=sj12.txt d=3 w=4096 p=128 i=64
- icms f=sj12.txt d=4 w=4096 p=128 i=64
- icms f=sj12.txt d=5 w=4096 p=128 i=64
- icms f=sj12.txt d=3 w=8192 p=128 i=64
- icms f=sj12.txt d=4 w=8192 p=128 i=64
- icms f=sj12.txt d=5 w=8192 p=128 i=64
- icms f=sj12.txt d=3 w=16384 p=128 i=64
- icms f=sj12.txt d=4 w=16384 p=128 i=64
- icms f=sj12.txt d=5 w=16384 p=128 i=64

**Chicago A**

- icms f=ch1.txt d=3 w=4096 p=128 i=64
- icms f=ch1.txt d=4 w=4096 p=128 i=64
- icms f=ch1.txt d=5 w=4096 p=128 i=64
- icms f=ch1.txt d=3 w=8192 p=128 i=64
- icms f=ch1.txt d=4 w=8192 p=128 i=64
- icms f=ch1.txt d=5 w=8192 p=128 i=64 
- icms f=ch1.txt d=3 w=16384 p=128 i=64
- icms f=ch1.txt d=4 w=16384 p=128 i=64
- icms f=ch1.txt d=5 w=16384 p=128 i=64

**Chicago B**

- icms f=ch2.txt d=3 w=4096 p=128 i=64
- icms f=ch2.txt d=4 w=4096 p=128 i=64
- icms f=ch2.txt d=5 w=4096 p=128 i=64
- icms f=ch2.txt d=3 w=8192 p=128 i=64
- icms f=ch2.txt d=4 w=8192 p=128 i=64
- icms f=ch2.txt d=5 w=8192 p=128 i=64
- icms f=ch2.txt d=3 w=16384 p=128 i=64
- icms f=ch2.txt d=4 w=16384 p=128 i=64
- icms f=ch2.txt d=5 w=16384 p=128 i=64

## License

MIT
