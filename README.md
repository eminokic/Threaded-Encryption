# CS 352 Project 2

## Multi-Threaded Encryption

## Description

The purpose of this project is to implement a multi-threaded text file encryptor. Conceptually,
the function of the program is simple: to read characters from an input source, encryptMessage the text,
and write the encrypted characters to an output. Also, the encryption program counts the number
of occurrences of each letter in the input and output files. All I/O and encryption is performed by
a module (encryptMessage-module.c) that is supplied by the project. Here is the single threaded version
that uses the methods of the encryptMessage module.

# Project Contents

## Main.c 
## Readme
## CMakeList.txt
## Testing
## Included Modules

# Starting The Project
```bash
1. cd src
2. make
3. ./encryptMessage inputFile outputFile
4. Provide Buffer Size: Integer Value
```
