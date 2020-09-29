/*
 * -----------------------------------------------------------------------------
 * Created by Ulysses Carlos on 09/28/2020 at 09:00 PM
 * 
 * Homework3.c
 * 
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include "Buffer.h"


buffer_item buffer[BUFFER_SIZE];


void *producer(void *param);
void *consumer(void *param);

//------------------------------------------------------------------------------
// Buffer_item function definitions
//------------------------------------------------------------------------------
int insert_item(buffer_item item){
    	/* Acquire Empty Semaphore */
	/* Acquire mutex lock to protect buffer */
	/* Insert item into buffer */
	/* Release mutex lock and full semaphore */
	return 0;
}

int remove_item(buffer_item *item){
    /* Acquire Full Semaphore */	
    /* Acquire mutex lock to protect buffer */
    /* remove an object from buffer placing it in item */
    /* Release mutex lock and empty semaphore */
	
	return 0;
}


void* producer(void *param){

    /* producer thread that calls insert_item() */
    return NULL;
}

void* consumer(void *param){

    /* consumer thread that calls remove_item() */
    return NULL;
}

bool str_isdigit(char *string){
    for (char *p = string; *p; p++)
	if (!isdigit(*p)) return false;
    return true;
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[]){
    /* Get command line arguments argv[1],argv[2],argv[3] */
    /* Initialize buffer related synchronization tools */
    /* 	Create producer threads based on the command line input */
    /* 	    Create consumer threads based on the command line input */
    /* 	    Sleep for user specified time based on the command line input */
	
    if (argc != 4){
	fprintf(stderr, "Usage: ./Homework3 [Sleep Time (s)] "
		"[# of Producer Threads] [# of Consumer Threads]\n");
	exit(EXIT_FAILURE);
    }
    
    // Prevent invalid input for arguments:
    bool check = str_isdigit(argv[1]) && str_isdigit(argv[2])
	&& str_isdigit(argv[3]);
    
    if (!check){
	fprintf(stderr, "Error: Argument(s) contain nonnumerical characters.\n");
	exit(EXIT_FAILURE);
    }

    int sleep_time = atoi(argv[1]);
    int producer_num = atoi(argv[2]);
    int consumer_num = atoi(argv[3]);

    if (sleep_time < 0){
	fprintf(stderr, "Error: Cannot have a negative sleep time.\n");
	exit(EXIT_FAILURE);
    }

    if (producer_num < 1 || consumer_num < 1){
	fprintf(stderr, "Error: There must be at least one producer and consumer"
		" thread.\n");
	exit(EXIT_FAILURE);
    }

    // Now do the stuff
}
