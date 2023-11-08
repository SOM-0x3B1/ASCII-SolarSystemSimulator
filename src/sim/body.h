#ifndef ASCII_SSS_BODY_H
#define ASCII_SSS_BODY_H


#include "../lib/econio.h"
#include "../vector.h"
#include "../graphics/layer.h"


typedef struct Trail{
    Point position;
    struct Trail *next;
} Trail;

typedef struct TrailQueue{
    Trail *head;
    int length;
    int capacity;
} TrailQueue;


typedef struct Body{
    char name[13];
    Vector position;
    Vector velocity;
    double r;
    double mass;
    char color;
    TrailQueue *trail;
} Body;


#include "body_array.h"


extern Body *sun;
extern Body *following;
extern Body *editedBody;

extern double solarMass;

extern double detectCollisionPercentage;

extern int trail_spacing_counter;


/*extern bool showDeatils;
extern bool showGRange;*/


int body_init();

Body *body_new(char *name, Vector pos, Vector v, double r, double mass, char color);

void body_addGravityEffect(Body *dest, Body const *src);

void body_move(Body *body);

void body_detectCollision(Body *a, Body *b);

// void body_draw(Body const *body);

void body_render();


void trailQueue_init(TrailQueue *tq, Body *b);

void trail_enqueue(TrailQueue *tq, Vector v);

void trailQueue_clear(TrailQueue *tq);


#endif //ASCII_SSS_BODY_H