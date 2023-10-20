#ifndef ASCII_SSS_BODY_H
#define ASCII_SSS_BODY_H

#include "../lib/econio.h"
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


Body extern sun;


void body_sun_init();

void body_move(Body *body);

// void body_draw(Body const *body);

void body_render();


#endif //ASCII_SSS_BODY_H