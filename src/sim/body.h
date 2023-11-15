#ifndef ASCII_SSS_BODY_H
#define ASCII_SSS_BODY_H


#include "../structs.h"
#include "../lib/econio.h"
#include "../vector.h"
#include "../graphics/layer.h"
#include "simulator.h"
#include "body_array.h"


typedef enum BodyEditableProperty {
    BODY_PROPERTY_NAME,
    BODY_PROPERTY_MASS,
    BODY_PROPERTY_R,
    BODY_PROPERTY_POS,
    BODY_PROPERTY_VEL,
    bodyEditableProperty_MAX
} BodyEditableProperty;



/** Allocates the body array, and creates a sun.*/
int body_init(Simulation *sim);

/**
 * Creates a new body.
 * @param v velocity
 * @param r radius
 * @param mass relative to Earth's mass (1: Earth's mass)
 * @return The pointer to the new body
 */
Body *body_new(char *name, Vector pos, Vector v, double r, double mass, char color, Simulation *sim);

/** Adds the gravitational effect of the src body to the destination body. */
void body_addGravityEffect(Body *dest, Body const *src, Simulation *sim);

/** Move a body by its velocity. */
void body_move(Body *body);

/** Detect if two bodies collide. */
void body_detectCollision(Body *a, Body *b, Simulation *sim);

/** Renders all bodies. */
void body_render(LayerInstances *li, Simulation *sim, Screen *screen);


/** Initializes a trail queue for a body. */
void trailQueue_init(TrailQueue *tq, Body *b);

/** Adds a new trail point to a trail queue. */
void trail_enqueue(TrailQueue *tq, Vector v);

/** Clears and frees a trail queue. */
void trailQueue_clear(TrailQueue *tq);


#endif //ASCII_SSS_BODY_H