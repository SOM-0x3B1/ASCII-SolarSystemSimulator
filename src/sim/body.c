#include <stdlib.h>
#include "body.h"
#include "../graphics/render.h"
#include "../global.h"


void body_addGravityEffect(Body *dest, Body src){
    double d = vector_distance(dest->position, src.position);
    double d2 = d * d;
    double force = src.mass / d2;

    Vector v = vector_subtract(dest->position, src.position);
    Vector unitVector = vector_scalarDivide(v, d);
    v = vector_scalarMultiply(unitVector, force);

    dest->velocity = vector_add(dest->velocity, v);
}

void body_move(Body body){
    body.velocity = vector_add(body.position, body.velocity);
}

void body_draw(Body body){
    Point p = vector_toPoint(body.position);
    p.y /= 2;
    p = point_scalarSubtract(p, screen_offset);
    for (int y = 0; y < screen_height; y++) {
        for (int x = 0; x < screen_width; x++) {
            int dX = x - p.x;
            int dY = (y - p.y) * 2;

            if ((dX * dX) + (dY * dY) <= (body.size * body.size))
                layer_writeAtXY(&bodyLayer, x, y, '@', body.color, body.color);
        }
    }
}