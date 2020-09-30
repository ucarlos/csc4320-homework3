/*
 * -----------------------------------------------------------------------------
 * Created by Ulysses Carlos on 09/29/2020 at 02:39 PM
 * 
 * Homework3.cc
 * I will test everything in C++ then port it back to C
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <string>
#include <array>
#include <pthread.h>
#include <semaphore.h>
#include "./Buffer.h"
#include <cstdlib>
#include <ctime>
using namespace std;


//Undefine macros for this program
#undef CONSUMER_QUEUE_END
#undef PRODUCER_QUEUE_END

#define SUCCESS (0)
#define FAILURE (1)
#define SLEEP_MAX (10)
#define MAX_VAL (10)
#define MIN_VAL (1)

array<buffer_item, BUFFER_SIZE> dequeue;

const buffer_item producer_queue_end = BUFFER_SIZE - 1;
const buffer_item consumer_queue_end = 0;

buffer_item current_producer{consumer_queue_end};
buffer_item current_consumer{consumer_queue_end};

//------------------------------------------------------------------------------
// Function declarations
//------------------------------------------------------------------------------
void print_dequeue(array<buffer_item, BUFFER_SIZE> &dequeue);



//------------------------------------------------------------------------------
// Dequeue declaration and definitions
//------------------------------------------------------------------------------
bool dequeue_is_empty(const array<buffer_item, BUFFER_SIZE> &deq){
    for (auto &i : deq)
	if (i != empty_val) return false;
    return true;
}



//------------------------------------------------------------------------------
// find_farthest_index(): Search for the farthest index where the producer
// can insert an item into the dequeue.
//------------------------------------------------------------------------------
int find_farthest_index(const array<buffer_item, BUFFER_SIZE> &dequeue){

    for (int i = producer_queue_end; i >= 0; i--)
    	if (dequeue.at(i) != empty_item) return i + 1;

    return BUFFER_SIZE; // If the dequeue is full.
}

//------------------------------------------------------------------------------
// find_closest_index(): Search for the closest index where the consumer
// can remove an item from the dequeue.
//------------------------------------------------------------------------------
int find_closest_index(const array<buffer_item, BUFFER_SIZE> &dequeue){

    for (int i = consumer_queue_end; i < dequeue.size(); i++)
	if (dequeue.at(i) != empty_item) return i - 1;
}
//------------------------------------------------------------------------------
// insert_item(): The producer inserts an item into the deque. If successful,
// the function returns 0. Otherwise it returns -1.
//------------------------------------------------------------------------------
int insert_item(buffer_item item){
    if (find_farthest_index(dequeue) == BUFFER_SIZE){
	fprintf(stderr, "Error: Cannot insert another item into this queue.\n");
	return FAILURE;
    }
    
    if (dequeue_is_empty(dequeue)){
	// Insert into the first slot.
	dequeue.at(consumer_queue_end) = item;
	current_producer = consumer_queue_end + 1;
	return SUCCESS;
    }
    
    static bool same_distance = ((current_consumer - consumer_queue_end) ==
			  (producer_queue_end - current_producer));
    
    if (same_distance){
	// Remove the left most item, and then insert from the right.
	// dequeue.at(current_consumer) = empty_val; // Consumer handles that
	dequeue.at(current_producer++) = item;
	return SUCCESS;
    }
    else {
	dequeue.at(current_producer++) = item;
	return SUCCESS;
    }

}

//------------------------------------------------------------------------------
// remove_item(): The consumer removes 
//------------------------------------------------------------------------------
int remove_item(buffer_item *item){
    if (dequeue_is_empty(dequeue)){
	fprintf(stderr, "ERROR: Cannot consume in a empty dequeue.\n");
	current_consumer = consumer_qeueue_end;
	return FAILURE;
    }

    static bool same_distance = ((current_consumer - consumer_queue_end) ==
				 (producer_queue_end - current_producer));
    if (same_distance){
	// Remove the left most item, and then insert from the right.
	*item = dequeue.at(current_consumer);
	dequeue.at(current_consumer) = empty_val;
	return SUCCESS;
    }

}
//------------------------------------------------------------------------------
// Producer():
//------------------------------------------------------------------------------
void * producer(void *arg){
    buffer_item item;
    while (true) {
	// Sleep for random period of time
	sleep(rand() % SLEEP_MAX);
	// Generate random number:
	item = rand() % MAX_VAL + MIN_VAL;

	if (!insert_item(item)){
	    printf("Producer %u: Produced %d at Position %d\t",
		   (unsigned) pthread_self(), item, current_producer - 1);
	    print_dequeue(dequeue);
	}
    }


    
}

//------------------------------------------------------------------------------
// Consumer(): 
//------------------------------------------------------------------------------
void * consumer(void *arg){
    buffer_item item;

    while (true) {
	// Sleep for random period of time
	sleep(rand() % SLEEP_MAX);
	if (!remove_item(item)){
	    printf("Consumer %u: Consumed %d at Position %d\t",
		   (unsigned) pthread_self(), item);
	    // Now print array:
	    print_dequeue(dequeue);
	}

    }


}

//------------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------------

bool str_isdigit(char *str){
    for (char *p = str; *p; p++)
	if (!isdigit(*p)) return false;
    return true;
}


void print_dequeue(array<buffer_item, BUFFER_SIZE> &dequeue){
    cout << "[ ";
    for (auto &i : dequeue)
	cout << i << " ";
    cout << "]\n";
}

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
    srand(time(nullptr));
    
    // Create Producer and Consumer arrays:
    array<pthread_t, producer_num> producer_list;
    array<pthread_t, consumer_num> consumer_list;
    
    // Sleep for specified time
    sleep(sleep_time);
    
}
