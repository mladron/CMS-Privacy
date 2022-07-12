# CMS-Privacy
On the Privacy of the Count-Min Sketch

## Description
The goal of Fingerprint Count Min Sketch (FCMS) is to reduce counter overestimation by adding a fingerprint to the counters and use it to check if consecutive increments to a counter belong to the same element. This code is used in the paper "Improving Packet Flow Counting with Fingerprint Counting" by Pedro Reviriego, Jorge Martinez and Salvatore Pontarelli, IEEE Communications Letters (in press).

The FCMS code used Daniel Alabi's Count-Min Sketch as a starting point https://github.com/alabid/countminsketch

## Compilation
This Visual Studio C++ project consists of 13 files including header, source files and the main program.

## Command line arguments
Command line arguments are:
- w: table width, default value is w=1048576
- d: table depth or number of counters checked, default value is d=2
- f: input keys
- a: heavy hitter selection, default value is 2 (maximum estimate)

**example: icms.exe w=16384 d=4, f=ch1.txt**

## License

MIT
