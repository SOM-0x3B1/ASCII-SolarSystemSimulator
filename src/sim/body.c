#include "body.h"

void body_addGravityEffect(Body *dest, Body src){
    double d = vector_distance(dest->position, src.position);
    double d2 = d *d;
    double force = src.mass / d2;

    Vector v = vector_subtract(dest->position, src.position);
    Vector egysegVektor = vector_scalarDivision(v, d);
    v = vector_scalarMultiply(egysegVektor, force);

    dest->velocity = vector_add(dest->velocity, v);
}

void body_move(Body body){
    body.velocity = vector_add(body.position, body.velocity);
}