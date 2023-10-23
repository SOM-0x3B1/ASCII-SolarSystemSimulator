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
    double mass;
    char color;
} Body;

typedef struct Trail{
    Vector position;
} Trail;


#include "body_array.h"


extern Body *sun;
extern Body *following;

extern double solarMass;

/*extern bool showDeatils;
extern bool showGRange;*/


int body_init();

Body *body_new(char *name, Vector pos, Vector v, int r, double mass, char color);

void body_addGravityEffect(Body *dest, Body const *src);

void body_move(Body *body);

// void body_draw(Body const *body);

void body_render();


#endif //ASCII_SSS_BODY_H