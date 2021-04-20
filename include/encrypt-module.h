#ifndef ENCRYPT_H
#define ENCRYPT_H

/**
 * TODO:
 * You must implement this function.
 * When the function returns the encryption module is allowed to reset.
 */
void reset_requested();

/**
 * TODO:
 * You must implement this function.
 * The function is called after the encryption module has finished a reset.
 */
void reset_finished();

/**
 * TODO:
 * You must use these functions to perform all I/O, encryption and counting
 * operations.
 */
void init(char *inputFileName, char *outputFileName);
int read_input(); // Done: To be Checked
void write_output(int c); // Done: To be Checked
int caesar_encrypt(int c); // Done: To be Revised
void count_input(int c); // Done: To be Checked
void count_output(int c); // Done: To be Checked
int get_input_count(int c);
int get_output_count(int c);
int get_input_total_count();
int get_output_total_count();

#endif // ENCRYPT_H
