#ifndef ASCII_SSS_BODY_H
#define ASCII_SSS_BODY_H

#endif //ASCII_SSS_BODY_H

#include "../econio/econio.h"
#include "vector.h"


typedef struct Body{
    int index;
    Vector position;
    int size;
    int mass;
    EconioColor color;

    Vector velocity;
} Body;

typedef struct Trail{
    Vector position;
} Trail;