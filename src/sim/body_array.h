#ifndef ASCII_SSS_BODY_ARRAY_H
#define ASCII_SSS_BODY_ARRAY_H


#include "../structs.h"
#include "body.h"


/** Allocates memory for the body array. */
int bodyArray_init(BodyArray *ba);

/** Frees the memory of the body array. */
void bodyArray_dispose(BodyArray *ba);

/** Adds a new body to the array. */
Body *bodyArray_add(BodyArray *ba, Body *b, Simulation *sim);

/** Remove a body at a given index. */
void bodyArray_removeAt(BodyArray *ba, int i, Simulation *sim);

/**
 * Removes a body.
 * @param b The pointer of the body.
 */
void bodyArray_remove(BodyArray *ba, Body *b, Simulation *sim);


#endif //ASCII_SSS_BODY_ARRAY_H
