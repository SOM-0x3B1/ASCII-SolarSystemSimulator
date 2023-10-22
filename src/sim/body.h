#ifndef ASCII_SSS_BODY_H
#define ASCII_SSS_BODY_H

#include "../lib/econio.h"
#include "../vector.h"
#include "../graphics/layer.h"


typedef struct Body{
    char name[13];
    Vector position;
    Vector velocity;
    int r;
    int mass;
    char color;
} Body;

typedef struct Trail{
    Vector position;
} Trail;


typedef struct BodyNode{
    Body body;
    struct BodyNode *next;
} BodyNode;


Body extern sun;

Body extern *following;

extern bool showDeatils;
extern bool showGRange;


void body_sun_init();

void body_move(Body *body);

// void body_draw(Body const *body);

void body_render();


#endif //ASCII_SSS_BODY_H