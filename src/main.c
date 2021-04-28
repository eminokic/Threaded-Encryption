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

    init(argv[1], argv[2]);

    printf("Enter the requested buffer size: ");
    fflush(stdout);

    char buff_size[256];
    fgets(buff_size, 256, stdin);
    buff_size[strlen(buff_size) - 1] = '\0';

    int size = atoi(buff_size);

    input_buffer.max = size;
    input_buffer.current = 0;
    output_buffer.max = size;
    output_buffer.current = 0;

    sem_init(&read_in, 0, 1);
    sem_init(&write_out, 0, 0);
    sem_init(&input_count, 0, 0);
    sem_init(&output_count, 0, 0);
    sem_init(&caesar_encrypt, 0, 0);
    sem_init(&caesar_encrypt, 0, 1);

    pthread_create(&in, NULL, read_input, NULL);
    pthread_create(&count_in, NULL, count_input, NULL);
    pthread_create(&encrypt, NULL, caesar_encrypt, NULL);
    pthread_create(&count_out, NULL, count_output, NULL);
    pthread_create(&out, NULL, write_output, NULL);

    pthread_join(in, NULL);
    pthread_join(count_in, NULL);
    pthread_join(encrypt, NULL);
    pthread_join(count_out, NULL);
    pthread_join(out, NULL);

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
    for (int i = 0; i < 256; i++) {
        if (((char) i) != '\n' && input_counts[i] > 0) {
            printf("%c: %d\n", (char) i, input_counts[i]);
        }
    }

    printf("Output File Contents: \n");
    //print out the output file contents
    for (int i = 0; i < 256; i++) {
        if (((char) i) != '\n' && output_counts[i] > 0){
            printf("%c: %d\n", (char) i, output_counts[i]);
        }
    }
}

int toLog = 0;

/**
 * The Response Log as the name states is used for logging responses made by the console during encryption.
 * Specifically, it is beneficial to log responses that the console makes regarding buffersize and segmentation faults.
 */
void responseLog(char *text) {
    if (toLog) {
        printf("%s",text);
    }
}
void *input_encrypt(void *a){

    node *curr;
    node *temp;

    int initial_s = 1;

    for(;;) {

        sem_wait(&encrypt_input);
        responseLog("input encryption\n");

        curr = input_buffer.front;

        while (NULL != curr) {

            if (curr->has_been_counted && !curr->has_been_encrypted) {
                if (curr->c != EOF && curr->c != '\n') {
                    curr->c = caesar_encrypt(curr->c);
                }
                curr->has_been_encrypted = 1;
                responseLog("encrypted input\n");
                break;
            }
            curr = curr->past;
        }
        if (input_buffer.current > 0 && input_buffer.front->has_been_encrypted) {
            temp = dequeue(&input_buffer);
            sem_post(&read_in);
        }
        sem_wait(&encrypt_output);
        enqueue(&output_buffer, temp->c);
        responseLog("Sent to output\n");

        sem_post(&output_count);

        if (temp->c == EOF) {
            responseLog("Encrypting done\n");
            break;
        }
    }
}

