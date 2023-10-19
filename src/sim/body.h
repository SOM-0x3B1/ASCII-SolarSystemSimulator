#ifndef ASCII_SSS_BODY_H
#define ASCII_SSS_BODY_H

#include "../econio/econio.h"
#include "../vector.h"
#include "../graphics/layer.h"


typedef struct Body{
    char name[13];
    Vector position;
    int r;
    int mass;
    EconioColor color;

    Vector velocity;
} Body;

typedef struct Trail{
    Vector position;
} Trail;


typedef struct BodyNode{
    Body body;
    struct BodyNode *next;
} BodyNode;


void body_draw(Body const *body);


#endif //ASCII_SSS_BODY_H