/*
 * -----------------------------------------------------------------------------
 * Created by Ulysses Carlos on 09/28/2020 at 09:00 PM
 * 
 * hw3.c
 * 
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "Buffer.h"


buffer_item buffer[BUFFER_SIZE];


void *producer(void *param);
void *consumer(void *param);

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

int main(int argc, char *argv[]){
    /* Get command line arguments argv[1],argv[2],argv[3] */
    /* Initialize buffer related synchronization tools */
    /* 	Create producer threads based on the command line input */
    /* 	    Create consumer threads based on the command line input */
    /* 	    Sleep for user specified time based on the command line input */
	
	
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
