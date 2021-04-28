/***************************************************************************//**

  @file         main.c

  @author       Emin Okic

  @date         Friday, April 23rd, 2021

  @Project        Multi-Threaded Encryption

*******************************************************************************/

#include "../include/encrypt-module.c"
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

void display_counts();

/**
 * A node object to be implemented into the queue.
 */
typedef struct node node;

struct node {

    char c;
    int has_been_counted;
    int has_been_encrypted;

    node* past;

};

/**
   Implementation of a queue for the buffer
 */
typedef struct {

    node* front;
    node* back;

    int max;
    int current;

} priority_queue;

int enqueue(priority_queue *queue, char c) {
    if (queue->current == queue->max) {
        return 0;
    }

    node* ch = (node*) malloc(sizeof(node));
    ch->has_been_counted = 0;
    ch->has_been_encrypted = 0;
    ch->c = c;

    if (queue->current == 0) {
        queue->front = ch;
        queue->back = ch;
    } else {
        queue->back->past = ch;
        queue->back = ch;
    }
    queue->current++;
    return 1;
}

node *dequeue(priority_queue *queue) {
    if (queue->current == 0) {
        return (node*) NULL;
    }

    node *n = (node*) malloc(sizeof(node));
    n->c = queue->front->c;
    n->has_been_counted = queue->front->has_been_counted;
    n->has_been_encrypted = queue->front->has_been_encrypted;

    queue->front = queue->front->past;

    if (queue->current == 1) {
        queue->back = NULL;
    }
    queue->current--;
    return n;
}

//Initializing input and output buffer variables
priority_queue input_buffer;
priority_queue output_buffer;

//Initializing Semaphores
sem_t read_in;
sem_t write_out;
sem_t input_count;
sem_t output_count;
sem_t encrypt_input;
sem_t encrypt_output;

/**
 * Main Function for running encryption via console.
 * @param argc
 * @param argv
 * @return program
 */
int main(int argc, char **argv) {

    //The list of threads for the main program
    pthread_t in, count_in, encrypt, count_out, out;

    //Command Input Verification
    if (argc != 3) {
        printf("Invalid number of arguments.\n   Usage: ./encrypt input-file output-file \n");
        exit(-1); //exit with an error
    }

    printf("Enter the requested buffer size: ");
    fflush(stdout);

    char buff_size[256];
    fgets(buff_size, 256, stdin);

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