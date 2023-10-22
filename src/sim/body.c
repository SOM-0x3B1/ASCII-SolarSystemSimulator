#include <stdlib.h>
#include <stdbool.h>
#include "body.h"
#include "../graphics/render.h"
#include "../global.h"
#include <string.h>
#include "../graphics/drawing.h"
#include "../graphics/layer.h"
#include "math.h"


Body sun;

Body *following = &sun;

bool showDeatils = true;
bool showGRange = true;


void body_sun_init() {
    strcpy(sun.name, "Sun");
    sun.color = '@';
    sun.r = 7;
    sun.mass = 20;
    Vector pos = {40, 30};
    sun.position = pos;
    Vector v = {0.1, 0.1};
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

    if(following == body) {
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
    if(showDeatils) {
        Point p = vector_toPoint(body->position);
        p.y /= 2;
        p = point_subtract(p, screen_offset);
        drawing_drawText(&bodyLayer, (int)(p.x - strlen(body->name) / 2), (int)p.y, body->name);
    }
}

void body_draw(Body const *body){
    Point p = vector_toPoint(body->position);
    p.y /= 2;
    p = point_subtract(p, screen_offset);
    for (int y = 0; y < screen_height; y++) {
        for (int x = 0; x < screen_width; x++) {
            long long int dX = x - p.x;
            long long int dY = (y - p.y) * 2;

            long long int dx2dy2 = (dX * dX) + (dY * dY);
            double er = body->r + body->mass * 1.2;

            long long int drange = llabs((dx2dy2 / 2) - (long long int)(er * er));

            if (dx2dy2 <= (body->r * body->r))
                layer_writeAtXY(&bodyLayer, x, y, '@');
            else if(showGRange && drange < (long long int)(er * 0.8))
                layer_writeAtXY(&bodyLayer, x, y, '.');
        }
    }
    body_drawInfo(body);
}

void body_render(){
    layer_clear(&bodyLayer);
    body_draw(&sun);
}