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
sem_t *producer_semaphore;
sem_t *consumer_semaphore;
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

//------------------------------------------------------------------------------
// Function declarations
//------------------------------------------------------------------------------
void print_dequeue(array<buffer_item, BUFFER_SIZE> &dequeue);


//------------------------------------------------------------------------------
// Functions to print producer and consumer messages
//------------------------------------------------------------------------------

void print_producer_message(buffer_item item, int position){
    cerr << "Producer " << pthread_self() << "produced " << item
	 << " at position " << position << "\t";
    print_dequeue(dequeue);
    cerr << endl;
}

void print_consumer_message(buffer_item item, int position){
    cerr << "Consumer " << pthread_self() << "consumed " << item
	 << " at position " << position << "\t";
    print_dequeue(dequeue);
    cerr << endl;
}

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
        
    if (dequeue_is_empty(dequeue)){
	// Insert into the first slot.
	dequeue.at(consumer_queue_end) = item;

	// printf("Producer %lu produced %d at position %d\n",
	//        pthread_self(), item, consumer_queue_end);
	print_producer_message(item, consumer_queue_end);

	current_producer = consumer_queue_end + 1;
	return SUCCESS;
    }
    else if (find_farthest_index(dequeue) == BUFFER_SIZE){
	fprintf(stderr, "Error: Cannot insert another item into this queue.\n");
	print_dequeue(dequeue);	
	return FAILURE;
    }    
    
    static bool same_distance = ((current_consumer - consumer_queue_end) ==
				 (producer_queue_end - current_producer));
    
    if (same_distance){
	// Remove the left most item, and then insert from the right.
	// dequeue.at(current_consumer) = empty_val; // Consumer handles that
	dequeue.at(current_producer++) = item;
	// cerr << "Producer " << pthread_self() << "produced " << item
	//      << " at position " << current_producer - 1 << "\t";
	// print_dequeue(dequeue);
	// cerr << endl;
	print_producer_message(item, current_producer - 1);
	return SUCCESS;
    }
    else {
	dequeue.at(current_producer++) = item;
	// cout << "Producer " << pthread_self() << "produced " << item
	//      << " at position " << current_producer - 1 << "\t";
	// print_dequeue(dequeue);
	// cout << endl;
	print_producer_message(item, current_producer - 1);
	return SUCCESS;
    }

}

//------------------------------------------------------------------------------
// remove_item(): The consumer removes 
//------------------------------------------------------------------------------
int remove_item(buffer_item *item){
    if (dequeue_is_empty(dequeue)){
	fprintf(stderr, "ERROR: Cannot consume in a empty dequeue.\n");
	print_dequeue(dequeue);
	current_consumer = consumer_queue_end;
	return FAILURE;
    }

    static bool same_distance = ((current_consumer - consumer_queue_end) ==
				 (producer_queue_end - current_producer));
    if (same_distance){
	// Remove the left most item, and then insert from the right.
	*item = dequeue.at(current_consumer);
	dequeue.at(current_consumer) = empty_val;
	print_consumer_message(*item, current_consumer);
	
	return SUCCESS;
    }

    return FAILURE;
}
//------------------------------------------------------------------------------
// Producer():
//------------------------------------------------------------------------------
void * producer(void *arg){
    buffer_item item;
    int sleep_time, check;
    while (true) {
	// Sleep for random period of time
	// sem_wait(semaphore);
	
	sleep_time = rand() % SLEEP_MAX;
	pthread_mutex_lock(&buffer_mutex);
	cerr << "Producer " << pthread_self() << " "
	     << "Sleeping for " << sleep_time << " seconds.\n";
	pthread_mutex_unlock(&buffer_mutex);
	// sem_post(semaphore);
	sleep(sleep_time);
	// Generate random number:



	item = rand() % MAX_VAL + MIN_VAL;
	// Acquire the semaphore
	check = insert_item(item);
	
	// Most of the issue comes here
	sem_wait(producer_semaphore);
	pthread_mutex_lock(&buffer_mutex);
	if (!insert_item(item)){
	    // printf("Producer %u: Produced %d at Position %d\t",
	    // 	   (unsigned) pthread_self(), item, current_producer - 1);
	    // print_dequeue(dequeue);
	}
	else
	    cerr << "Something went wrong with insert_item\n";
		

	// Release the mutex and then the semaphore
	pthread_mutex_unlock(&buffer_mutex);
	sem_post(producer_semaphore);
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
	pthread_mutex_lock(&buffer_mutex);
	cerr << "Consumer " << pthread_self() << " sleeping for "
	     << sleep_time << " seconds.\n";
	pthread_mutex_lock(&buffer_mutex);
	// sem_post(semaphore);
	sleep(sleep_time);
	// Acquire the semaphore

	sem_wait(consumer_semaphore);
	pthread_mutex_lock(&buffer_mutex);
	if (!remove_item(&item)){
	    printf("Consumer %u: Consumed %d at Position %d\t",
		   (unsigned) pthread_self(), (int)item, current_consumer);
	    // Now print array:
	    print_dequeue(dequeue);
	}
	else
	    cerr << "Something went wrong with remove_item\n";
	// Release the lock
	pthread_mutex_unlock(&buffer_mutex);
	// Release the semaphore
	sem_post(consumer_semaphore);


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
    cout << "]";
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

    // Fill the vector with empty_character:
    for (auto &i : dequeue)
	i = empty_val;
    
    //Initialize the semaphore and any mutexes:
    producer_semaphore = sem_open("HW3: Producer Semaphore",
				  O_CREAT, 0666, BUFFER_SIZE);
    
    consumer_semaphore = sem_open("HW3: Consumer Semaphore",
				  O_CREAT, 0666, 0);

    
    pthread_mutex_init(&buffer_mutex, nullptr);
    
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
    
    sleep(sleep_time);

    // Now close each list
    for (int i = 0; i < producer_num; i++)
	pthread_join(producer_list[i], nullptr);

    for (int i = 0; i < consumer_num; i++)
	pthread_join(consumer_list[i], nullptr);
    // Sleep for specified time and exit
    
    sem_destroy(producer_semaphore);
    sem_destroy(consumer_semaphore);
}
