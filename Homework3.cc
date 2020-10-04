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
#include <vector>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include "./Buffer.h"
#include <cstdlib>
#include <ctime>
#include <unistd.h>
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

//
sem_t *semaphore;
pthread_mutex_t producer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t consumer_mutex = PTHREAD_MUTEX_INITIALIZER;
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
    	if (dequeue.at(i) != empty_val) return i + 1;

    return BUFFER_SIZE; // If the dequeue is full.
}

//------------------------------------------------------------------------------
// find_closest_index(): Search for the closest index where the consumer
// can remove an item from the dequeue.
//------------------------------------------------------------------------------
int find_closest_index(const array<buffer_item, BUFFER_SIZE> &dequeue){

    for (int i = consumer_queue_end; i < dequeue.size(); i++)
	if (dequeue.at(i) != empty_val) return i - 1;

    return -1;
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
	current_consumer = consumer_queue_end;
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

    return FAILURE;
}
//------------------------------------------------------------------------------
// Producer():
//------------------------------------------------------------------------------
void * producer(void *arg){
    buffer_item item;
    int sleep_time;
    while (true) {
	// Sleep for random period of time
	// sem_wait(semaphore);
	
	sleep_time = rand() % SLEEP_MAX;
	pthread_mutex_lock(&producer_mutex);
	cerr << "Producer " << pthread_self() << " "
	     << "Sleeping for " << sleep_time << " seconds.\n";
	pthread_mutex_unlock(&producer_mutex);
	// sem_post(semaphore);
	sleep(sleep_time);
	// Generate random number:


	sem_wait(semaphore);
	item = rand() % MAX_VAL + MIN_VAL;
	// Acquire the semaphore

	if (!insert_item(item)){
	    printf("Producer %u: Produced %d at Position %d\t",
		   (unsigned) pthread_self(), item, current_producer - 1);
	    print_dequeue(dequeue);
	}
	else
	    cerr << "Something went wrong with insert_item\n";
		

	// Release the semaphore
	sem_post(semaphore);
    }
    
}

//------------------------------------------------------------------------------
// Consumer(): 
//------------------------------------------------------------------------------
void * consumer(void *arg){
    buffer_item item;
    int sleep_time;
    while (true) {
	// Sleep for random period of time
	// sem_wait(semaphore);
	sleep_time = rand() % SLEEP_MAX;
	pthread_mutex_lock(&consumer_mutex);
	cerr << "Consumer " << pthread_self() << " sleeping for "
	     << sleep_time << " seconds.\n";
	pthread_mutex_lock(&consumer_mutex);
	// sem_post(semaphore);
	sleep(sleep_time);
	// Acquire the semaphore

	// sem_wait(semaphore);
	pthread_mutex_lock(&consumer_mutex);
	if (!remove_item(&item)){
	    printf("Consumer %u: Consumed %d at Position %d\t",
		   (unsigned) pthread_self(), (int)item, current_consumer);
	    // Now print array:
	    print_dequeue(dequeue);
	}
	else
	    cerr << "Something went wrong with remove_item\n";

	// Release the semaphore
	// sem_post(semaphore);
	pthread_mutex_unlock(&consumer_mutex);

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

    const int sleep_time = atoi(argv[1]);
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
    
    //Initialize the semaphore and any mutexes:
    semaphore = sem_open("Homework3_Semaphore", O_CREAT, 0666, 1);
    pthread_mutex_init(&producer_mutex, nullptr);
    pthread_mutex_init(&consumer_mutex, nullptr);
    
    // Now do the stuff
    srand(time(nullptr));

    cout << "Creating " << producer_num << " producer(s) and "
	 << consumer_num << " consumer(s).\n";
    // Create Producer and Consumer arrays:
    vector<pthread_t> producer_list(producer_num);
    vector<pthread_t> consumer_list(consumer_num);


    // Create pthreads for Producer
    

    for (int i = 0; i < producer_num; i++)
	pthread_create(&producer_list[i], nullptr, producer, nullptr);

    for (int i = 0; i < consumer_num; i++)
	pthread_create(&consumer_list[i], nullptr, consumer, nullptr);
    

    // Now close each list
    for (int i = 0; i < producer_num; i++)
	pthread_join(producer_list[i], nullptr);

    for (int i = 0; i < consumer_num; i++)
	pthread_join(consumer_list[i], nullptr);
    // Sleep for specified time and exit
    
    sleep(sleep_time);
    sem_destroy(semaphore);
}
