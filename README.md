# CS 352 Project 2

## Multi-Threaded Encryption

## Description

The purpose of this project is to implement a multi-threaded text file encryptor. Conceptually,
the function of the program is simple: to read characters from an input source, encrypt2 the text,
and write the encrypted characters to an output. Also, the encryption program counts the number
of occurrences of each letter in the input and output files. All I/O and encryption is performed by
a module (encrypt2-module.c) that is supplied by the project. Here is the single threaded version
that uses the methods of the encrypt2 module.

## Requirements 

Main does the following.
1. Obtain the input and output files as command line arguments. If the number of
command line arguments is incorrect, exit after displaying a message about correct usage.
2. Call init( ) with the input and output file names.
3. Prompt the user for the input and output buffer sizes N and M. The buffers may be any
size >1.
4. Initialize shared variables. This includes allocating appropriate data structures for the
input and output buffers. You may use any data structure capable of holding exactly N
and M characters for the input and output buffers respectively.
5. Create the other threads.
6. Wait for all threads to complete.
7. Display the number of occurrences of each letter in the input and output files.

# Project Contents

## Main.c 

# Starting The Project
```bash
cd src
make
./main
```
