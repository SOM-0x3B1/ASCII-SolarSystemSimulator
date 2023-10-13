#include "body.h"


void body_addGravityEffect(Body *dest, Body src){
    double d = vector_distance(dest->position, src.position);
    double d2 = d * d;
    double force = src.mass / d2;

    Vector v = vector_subtract(dest->position, src.position);
    Vector egysegVektor = vector_scalarDivision(v, d);
    v = vector_scalarMultiply(egysegVektor, force);

    dest->velocity = vector_add(dest->velocity, v);
}

void body_move(Body body){
    body.velocity = vector_add(body.position, body.velocity);
}

void body_draw(Body body){
    Point p = vector_toPoint(body.position);
    int v = p.y / 2;
    for (int y = 0; y < 30; y++) {
        for (int x = 0; x < 120; x++) {
            int dX = x - p.x;
            int dY = (y - v) * 2;

            if ((dX * dX) + (dY * dY) <= (body.size * body.size))
                layer_writeXY(bodyLayer, x, y, '#', body.color, body.color);
        }
    }
}