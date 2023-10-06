#ifndef ASCII_SSS_BODY_H
#define ASCII_SSS_BODY_H

#endif //ASCII_SSS_BODY_H

#include "../econio/econio.h"
#include "vector.h"


typedef struct Body{
    int index;
    Vektor position;
    int size;
    int mass;
    int hasShadow;
    EconioColor color;

    Vektor velocity;
} Body;

typedef struct Trail{
    Vektor position;
} Trail;

