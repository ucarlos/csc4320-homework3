/* buffer.h*/
#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE (5)


//------------------------------------------------------------------------------
// Dequeue Necessary constants and declarations
//------------------------------------------------------------------------------
typedef int buffer_item;
const buffer_item empty_val = -1;

// Dequeue Function declarations
int insert_item(buffer_item item);
int remove_item(buffer_item *item);

// Producer and Consumer Message Declarations:
void print_producer_message(buffer_item item, int position);
void print_consumer_message(buffer_item item, int position);

//------------------------------------------------------------------------------
// Language-specific function declarations
//------------------------------------------------------------------------------
#ifdef __cplusplus // C++ Function declarations (For Homework3.cc)

void print_dequeue(const std::array<buffer_item, BUFFER_SIZE> &dequeue);
int find_farthest_index(const std::array<buffer_item, BUFFER_SIZE> &dequeue);
int find_closest_index(const std::array<buffer_item, BUFFER_SIZE> &dequeue);
bool dequeue_is_empty(const std::array<buffer_item, BUFFER_SIZE> &deq);

#else // C Function declarations (For Homework3.c)
#include <stdbool.h>

void print_dequeue(const buffer_item *dequeue);
int find_farthest_index(const buffer_item *dequeue);
int find_closest_index(const buffer_item *dequeue);
bool dequeue_is_empty(const buffer_item *dequeue);

#endif

#endif
