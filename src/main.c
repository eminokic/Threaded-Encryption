/***************************************************************************//**

  @file         main.c

  @author       Emin Okic

  @date         Friday, April 23rd, 2021

  @Project        Multi-Threaded Encryption

*******************************************************************************/

#include "../include/encrypt-module.c"
#include <stdio.h>

void display_counts();

int main(int argc, char **argv) {

    //Command Input Verification
    if (argc != 3) {
        printf("Invalid number of arguments.\n   Usage: ./encrypt input-file output-file \n");
        exit(-1); //exit with an error
    }

    init(argv[1], argv[2]);
    while ((argc = read_input()) != EOF) {
        count_input(argc);
        argc = caesar_encrypt(argc);
        count_output(argc);write_output(argc);
    }
    printf("End of file reached.\n");
    display_counts();

    return 1;

}

void display_counts() {
    //print out the input file contents
    for (int i = 0; i < 255; i++) {
        if (((char) i) != '\n' && input_counts[i] > 0) {
            printf("%c: %d\n", (char) i, input_counts[i]);
        }
    }

    printf("Output File Contents: \n");
    //print out the output file contents
    for (int i = 0; i < 255; i++) {
        if (((char) i) != '\n' && output_counts[i] > 0){
            printf("%c: %d\n", (char) i, output_counts[i]);
        }
    }
}
