#ifndef ASCII_SSS_BODY_H
#define ASCII_SSS_BODY_H

#endif //ASCII_SSS_BODY_H

#include "../econio/econio.h"
#include "vector.h"
#include "../graphics/layer.h"


typedef struct Body{
    char name[12];
    Vector position;
    int size;
    int mass;
    EconioColor color;

    Vector velocity;
} Body;

typedef struct Trail{
    Vector position;
} Trail;


void body_draw(Body body);