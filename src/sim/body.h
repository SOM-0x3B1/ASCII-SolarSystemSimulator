#ifndef ASCII_SSS_BODY_H
#define ASCII_SSS_BODY_H


#include "../lib/econio.h"
#include "../vector.h"
#include "../graphics/layer.h"


/** A point of a trail; a queue element. */
typedef struct Trail{
    Point position;
    struct Trail *next;
} Trail;

/** A queue of trail points. */
typedef struct TrailQueue{
    Trail *head;
    int length;
    int capacity;
} TrailQueue;


/** A celestial body. */
typedef struct Body{
    char name[13];
    Vector position;
    Vector velocity;
    double r;
    double mass;
    char color;
    TrailQueue trail;
} Body;


#include "body_array.h"


extern Body *sun;
extern Body *following;  // The body that the camera follows (NULL, if the camera is free).
extern Body *editedBody; // The body that is currently being edited by the body editor.

extern double solarMass; // The mass of the sun; every gravitational effect is relative to this!

extern double detectCollisionPercentage; // The percentage of the minimum radius overlap that triggers a collision event.

extern int trail_spacing_counter; // Measures the time passed since the last trail point.


/** Allocates the body array, and creates a sun.*/
int body_init();

/**
 * Creates a new body.
 * @param v velocity
 * @param r radius
 * @param mass relative to Earth's mass (1: Earth's mass)
 * @return The pointer to the new body
 */
Body *body_new(char *name, Vector pos, Vector v, double r, double mass, char color);

/** Adds the gravitational effect of the src body to the destination body. */
void body_addGravityEffect(Body *dest, Body const *src);

/** Move a body by its velocity. */
void body_move(Body *body);

/** Detect if two bodies collide. */
void body_detectCollision(Body *a, Body *b);

/** Renders all bodies. */
void body_render();


/** Initializes a trail queue for a body. */
void trailQueue_init(TrailQueue *tq, Body *b);

/** Adds a new trail point to a trail queue. */
void trail_enqueue(TrailQueue *tq, Vector v);

/** Clears and frees a trail queue. */
void trailQueue_clear(TrailQueue *tq);


#endif //ASCII_SSS_BODY_H