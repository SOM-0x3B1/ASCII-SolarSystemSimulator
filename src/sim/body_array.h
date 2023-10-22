#ifndef ASCII_SSS_BODY_ARRAY_H
#define ASCII_SSS_BODY_ARRAY_H


#include "body.h"


typedef struct BodyArray{
    Body *data;
    int length;
    int capacity;
} BodyArray;


extern BodyArray bodyArray;


int bodyArray_init();

void bodyArray_dispose();

Body *bodyArray_add(Body b);


#endif //ASCII_SSS_BODY_ARRAY_H
