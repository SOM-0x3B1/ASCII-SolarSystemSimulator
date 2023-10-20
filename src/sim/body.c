#include <stdlib.h>
#include "body.h"
#include "../graphics/render.h"
#include "../global.h"
#include <string.h>
#include "../graphics/drawing.h"
#include "../graphics/layer.h"


Body sun;
Body *follow = &sun;

void body_sun_init() {
    strcpy(sun.name, "Sun");
    sun.color = COL_LIGHTYELLOW;
    sun.r = 7;
    sun.mass = 20;
    Vector pos = {40, 30};
    sun.position = pos;
    Vector v = {0.01, -0.01};
    sun.velocity = v;
}

void body_addGravityEffect(Body *dest, Body const *src){
    double d = vector_distance(dest->position, src->position);
    double d2 = d * d;
    double force = src->mass / d2;

    Vector v = vector_subtract(dest->position, src->position);
    Vector unitVector = vector_scalarDivide(v, d);
    v = vector_scalarMultiply(unitVector, force);

    dest->velocity = vector_add(dest->velocity, v);
}

void body_move(Body *body){
    body->position = vector_add(body->position, body->velocity);

    if(follow == body) {
        Point p = vector_toPoint(body->position);
        p.y /= 2;
        Point screenSize = {screen_width / 2, screen_height / 2};
        p = point_subtract(p, screenSize);
        if(menuLayer.enabled)
            p.x += 16;
        screen_offset = p;
    }
}


void body_drawInfo(Body const *body) {
    if(infoLayer.enabled) {
        Point p = vector_toPoint(body->position);
        p.y /= 2;
        p = point_subtract(p, screen_offset);
        drawing_drawText(&infoLayer, p.x - (int) strlen(body->name) / 2, p.y, body->name, COL_WHITE);
    }
}

void body_draw(Body const *body){
    Point p = vector_toPoint(body->position);
    p.y /= 2;
    p = point_subtract(p, screen_offset);
    for (int y = 0; y < screen_height; y++) {
        for (int x = 0; x < screen_width; x++) {
            int dX = x - p.x;
            int dY = (y - p.y) * 2;

            int dx2dy2 = (dX * dX) + (dY * dY);
            int er = body->r + body->mass;

            if (dx2dy2 <= (body->r * body->r))
                layer_writeAtXY(&bodyLayer, x, y, '@', body->color, body->color);
            else if(abs(dx2dy2 / 2 - er * er) < er * 0.8)
                layer_writeAtXY(&bodyLayer, x, y, '.', body->color, body->color);
        }
    }
    body_drawInfo(body);
}

void body_render(){
    layer_clear(&bodyLayer);
    layer_clear(&infoLayer);
    body_draw(&sun);
}