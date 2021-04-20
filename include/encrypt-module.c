#include "encrypt-module.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

FILE *input_file;
FILE *output_file;
int input_counts[256];
int output_counts[256];
int input_total_count;
int output_total_count;
int key = 1;

/**
 * Function: Clear Count (Void)
 * Description: Clears count of input based on size of input.
 */
void clear_counts() {
	memset(input_counts, 0, sizeof(input_counts));
	memset(output_counts, 0, sizeof(output_counts));
	input_total_count = 0;
	output_total_count = 0;
}

/**
 * Function: Random Reset (Void)
 * Description: Randomly resets a process by sleeping.
 */
void *random_reset() {
	while (1) {
		sleep(rand() % 11 + 5);
		reset_requested();
		key = rand() % 26;
		clear_counts();
		reset_finished();
	}
}


/**
 * Function: Init (Void)
 * Description:
 */
void init(char *inputFileName, char *outputFileName) {
	pthread_t pid;
	srand(time(0));
	pthread_create(&pid, NULL, &random_reset, NULL);
	input_file = fopen(inputFileName, "r");
	output_file = fopen(outputFileName, "w");
}

/**
 * Function: Read Input (Integer Value)
 * Description:
 */
int read_input() {
	usleep(10000);
	return fgetc(input_file);
}

/**
 * Function: Write Output
 * Description:
 */
void write_output(int c) {
	fputc(c, output_file);
}


/**
 * Function: Caesar Encrypt
 * Description: Encryption function using the caesar encryption method.
 */
int caesar_encrypt(int c) {
	if (c >= 'a' && c <= 'z') {
		c += key;
		if (c > 'z') {
			c = c - 'z' + 'a' - 1;
		}
	} else if (c >= 'A' && c <= 'Z') {
		c += key;
		if (c > 'Z') {
			c = c - 'Z' + 'A' - 1;
		}
	}
	return c;
}


/**
 * Function: Count Input (Void)
 * Description:
 */
void count_input(int c) {
	input_counts[toupper(c)]++;
	input_total_count++;
}


/**
 * Function: Count Output (Void)
 * Description:
 */
void count_output(int c) {
	output_counts[toupper(c)]++;
	output_total_count++;
}

/**
 * Function: Get Input Count (Integer Value)
 * Description:
 */
int get_input_count(int c) {
	return input_counts[toupper(c)];
}

/**
 * Function: Get Output Count (Integer Value)
 * Description:
 */
int get_output_count(int c) {
	return output_counts[toupper(c)];
}

/**
 * Function: Get Total Input Count (Integer Value)
 * Description:
 */
int get_input_total_count() {
	return input_total_count;
}


/**
 * Function: Get Total Output Count (Integer Value)
 * Description:
 */
int get_output_total_count() {
	return output_total_count;
}
