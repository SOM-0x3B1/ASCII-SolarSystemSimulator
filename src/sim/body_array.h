#ifndef ASCII_SSS_BODY_ARRAY_H
#define ASCII_SSS_BODY_ARRAY_H


#include "body.h"


typedef struct BodyArray{
    Body *data;
    int length;
    int capacity;
} BodyArray;


/** The dynamic array that holds all bodies. */
extern BodyArray bodyArray;


/** Allocates memory for the body array. */
int bodyArray_init();

/** Frees the memory of the body array. */
void bodyArray_dispose();

/** Adds a new body to the array. */
Body *bodyArray_add(Body *b);

/** Remove a body at a given index. */
void bodyArray_removeAt(int i);

/**
 * Removes a body.
 * @param b The pointer of the body.
 */
void bodyArray_remove(Body *b);


#endif //ASCII_SSS_BODY_ARRAY_H
