#include "body.h"

void body_addGravityEffect(Body *dest, Body *src){
    double d = vektor_tavolsag(&dest->position, &src->position);
    double d2 = d *d;
    double erosseg = src->mass / d2;

    Vektor v = vektor_kivon(&dest->position, &src->position);
    Vektor egysegVektor = vektor_skalarOszt(&v, d);
    v = vektor_skalarSzoroz(&egysegVektor, erosseg);

    dest->velocity = vektor_osszeAd(&dest->velocity, &src->velocity);
}

void body_move(Body body){
    body.velocity = vektor_osszeAd(&body.position, &body.velocity);
}