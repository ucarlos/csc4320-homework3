/* buffer.h*/
#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE (5)
// Struct Declaration
typedef int buffer_item;

// Struct Definition
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
#endif