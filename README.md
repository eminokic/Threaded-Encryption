# CS 352 Project 1

## UNIX Shell Interface

## Description
The objective is to implement a shell interface that accepts user commands and executes each 
command in a separate process. This shell will be implemented using the C programming language.

## Requirements 
The shell shall support file redirection using the '>' and '<' symbols. 

The shell shall support pipelining, denoted by the '|' symbol.

The shell shall support background commands, denoted by the '&' symbol. 
Note: Commands shall print out the following upon completing a command in the background. \

[Process_ID] Done "command"

The shell shall support the "jobs" command where executing jobs will display all background and foreground processes. 

The shell shall support the "control+z" command that stops the currently running foreground process.

The shell shall support the "bg [Process_ID]" command where executing bg followed by a stopped process will resume that process.

The shell shall support the "kill" command where "kill [Process_ID]" terminates the process and prints out "Terminated [Process_ID]".

# Project Contents

## Main.c 
Main.c runs a while loop that accepts input from user commands and outputs the correct response.

## Colors.h
An open source file found alone used solely for the purposes of implementing UI designs of the shell.
# Starting The Project
```bash
cd src
make
./main
```