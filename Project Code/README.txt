*********
DISKPROCESS

Overview: The diskcprocess.h/diskprocess.cpp implement a volatile, simulated disk.
The API to the disk include commands to read and to write blocks.
The number of blocks and the size of each block on the disk are
specified to the constructor. Blocks are only created when they are read or written.
A small level of performance measurement is done and can be written to a log file if logging is enabled.

The file Demo.cpp is an example of how to use the diskprocess API.

INSTALL
1. Copy demo.cpp, diskprocess.cpp, diskprocess.h and the Makefile to a folder on a linux machine.
2. cd to that folder
3. type 'make Demo' 
4. type ./Demo

Author: D.M. Lyons
Date: Spring 2017
c D. Lyons, Fordham CIS 2017
