/***************************************************************************//**

  @file         main.c

  @author       Emin Okic

  @date         Friday, April 23rd, 2021

  @Project        Multi-Threaded Encryption

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>

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
    int curr;

} priority_queue;

/**
   Encrypt Module Library
 */

void *count_input(void *a);
void *read_input(void *a);
void *input_encrypt(void *a);
void *count_output(void *a);
void *write_output(void *a);
void responseLog(char *text);


int in_count[255];
int out_count[255];
int size;

priority_queue input_buffer;
priority_queue output_buffer;

sem_t read_in;
sem_t write_out;
sem_t input_count;
sem_t output_count;
sem_t encrypt_input;
sem_t encrypt_output;

FILE *in_file;
FILE *out_file;

int toLog = 0;

int main(int argc, char **argv) {

    //The list of threads for the main program
    pthread_t in, count_in, encrypt, count_out, out;

    //Command Input Verification
    if (argc != 3) {
        printf("Invalid number of arguments.\n   Usage: ./encryptMessage [infile] [outfile] \n");
        exit(-1); //exit with an error
    }

    //Read/Write Files
    in_file = fopen(argv[1], "r");
    out_file = fopen(argv[2], "w");

    if (in_file != NULL) {

        printf("Please enter a buffer size: ");
        fflush(stdout);

        char buff_size[256];
        fgets(buff_size, 256, stdin);
        buff_size[strlen(buff_size) - 1] = '\0';

        size = atoi(buff_size);

        input_buffer.max = size;
        input_buffer.curr = 0;
        output_buffer.max = size;
        output_buffer.curr = 0;

        sem_init(&read_in, 0, 1);
        sem_init(&write_out, 0, 0);
        sem_init(&input_count, 0, 0);
        sem_init(&output_count, 0, 0);
        sem_init(&encrypt_input, 0, 0);
        sem_init(&encrypt_output, 0, 1);

        pthread_create(&in, NULL, read_input, NULL);
        pthread_create(&count_in, NULL, count_input, NULL);
        pthread_create(&encrypt, NULL, input_encrypt, NULL);
        pthread_create(&count_out, NULL, count_output, NULL);
        pthread_create(&out, NULL, write_output, NULL);

        pthread_join(in, NULL);
        pthread_join(count_in, NULL);
        pthread_join(encrypt, NULL);
        pthread_join(count_out, NULL);
        pthread_join(out, NULL);

        printf("input file contains: \n");

        //print out the input file contents
        for (int i = 0; i < 255; i++) {
            if (((char) i) != '\n' && in_count[i] > 0) {
                printf("%c: %d\n", (char) i, in_count[i]);
            }
        }

        printf("output file contains: \n");
        //print out the output file contents
        for (int i = 0; i < 255; i++) {
            if (((char) i) != '\n' && out_count[i] > 0){
                printf("%c: %d\n", (char) i, out_count[i]);
            }
        }

        return 1;

    } else {
        printf("File not found for given name: %s \n", argv[1]);
        exit(-1);
    }
}

int enqueue(priority_queue *queue, char c) {

    //check to see if the queue is full
    if (queue->curr == queue->max) {
        return 0;
    }

    node* ch = (node*) malloc(sizeof(node));
    ch->has_been_counted = 0;
    ch->has_been_encrypted = 0;
    ch->c = c;

    //if nothing is in the queue so far
    if (queue->curr == 0) {
        queue->front = ch;
        queue->back = ch;
    } else { //otherwise set ch's past equal to the back  and put ch at the back
        queue->back->past = ch;
        queue->back = ch;
    }
    queue->curr++;
    return 1;
}

node *dequeue(priority_queue *queue) {

    //nothing to remove from the queue
    if (queue->curr == 0) {
        return (node*) NULL;
    }

    //otherwise remove the front
    node *n = (node*) malloc(sizeof(node));
    n->c = queue->front->c;
    n->has_been_counted = queue->front->has_been_counted;
    n->has_been_encrypted = queue->front->has_been_encrypted;

    //"pop" the front of the queue off
    queue->front = queue->front->past;

    //check to see if there is only one element in the queue
    //and set the back equal to null if so
    if (queue->curr == 1) {
        queue->back = NULL;
    }
    queue->curr--; //decrement the num of nodes
    return n; //return the new node
}

/**
   Encryption Algorithm:
    1. s = 1;
    2. Get next character c.
    3. if c is not a letter then goto (7).
    4. if (s==1) then increase c with wraparound (e.g., 'A' becomes 'B', 'c' becomes 'd',
        'Z' becomes 'A', 'z' becomes 'a'), set s=-1, and goto (7).
    5. if (s==-1) then decrease c with wraparound (e.g., 'B' becomes 'A', 'd' becomes 'c',
        'A' becomes 'Z', 'a' becomes 'z'), set s=0, and goto (7).
    6. if (s==0), then do not change c, and set s=1.
    7. Encrypted character is c.
    8. If c!=EOF then goto (2).

 */

char encryptMessage(char c, int *s) {

    int int_value = (int) c;

    if ((int_value >= 65 && int_value <= 90) || (int_value >= 97 && int_value <= 122)) {

        switch(*s) {

            case -1:
                *s = 0;
                int_value = (int_value == 65) ? 90 :
                            (int_value == 97) ? 122 :
                            int_value - 1;
                return (char) int_value;
            case 0:
                *s = 1;
                return c;
            case 1:
                *s = -1;
                int_value = (int_value == 90) ? 65 :
                            (int_value == 122) ? 97 :
                            int_value + 1;
                return (char) int_value;
        }
    } else { return c; }
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
                    curr->c = encryptMessage(curr->c, &initial_s);
                }
                curr->has_been_encrypted = 1;
                responseLog("encrypted input\n");
                break;
            }
            curr = curr->past;
        }
        if (input_buffer.curr > 0 && input_buffer.front->has_been_encrypted) {
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

void *count_output(void *a) {

    node *curr;

    for(;;) {

        sem_wait(&output_count);
        curr = output_buffer.front;

        responseLog("output counting\n");
        while (NULL != curr) {

            if (!curr->has_been_counted) {

                out_count[curr->c]++;
                curr->has_been_counted = 1;
                sem_post(&write_out);
                responseLog("Counted output\n");

                if (curr->c == EOF) {

                    responseLog("Counting output done\n");
                    return (void*) NULL;
                } else {
                    break;
                }
            } else {
                curr = curr->past;
            }
        }
    }
}

void *count_input(void *a) {

    node *curr;

    for(;;) {

        sem_wait(&input_count);
        curr = input_buffer.front;
        responseLog("input counting\n");
        while (NULL != curr) {
            if (curr->has_been_counted == 0) {
                in_count[curr->c]++;
                curr->has_been_counted = 1;
                sem_post(&encrypt_input);
                responseLog("Counted input\n");

                if(curr->c == EOF) {
                    responseLog("Counting input done\n");
                    return (void*) NULL;
                } else {
                    break;
                }
            } else {
                curr = curr->past;
            }
        }
    }
}

void *read_input(void *a) {

    char curr;

    curr = fgetc(in_file);

    for(;;) {
        sem_wait(&read_in);

        if (enqueue(&input_buffer, curr)) {
            responseLog("Char put into queue\n");
            sem_post(&input_count);

            if(curr == EOF) {
                responseLog("Done reading input\n");
                break;
            } else {
                curr = fgetc(in_file);
            }
        }
    }
}

void *write_output(void *a) {

    node *curr;

    for(;;) {
        sem_wait(&write_out);
        curr = output_buffer.front;

        if (curr->has_been_counted) {
            dequeue(&output_buffer);

            if (curr->c == EOF) {
                break;
            }

            fputc(curr->c, out_file);
            fflush(out_file);
            curr = curr->past;
        }
        sem_post(&encrypt_output);
    }
    responseLog("Done writing output\n");
}

/**
 * The Response Log as the name states is used for logging responses made by the console during encryption.
 * Specifically, it is beneficial to log responses that the console makes regarding buffersize and segmentation faults.
 */
void responseLog(char *text) {
    if (toLog) {
        printf("%s",text);
    }
}
