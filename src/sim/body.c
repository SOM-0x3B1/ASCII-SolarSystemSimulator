#include <stdlib.h>
#include <stdbool.h>
#include "body.h"
#include "../graphics/render.h"
#include "../global.h"
#include <string.h>
#include "../graphics/drawing.h"
#include "../graphics/layer.h"
#include "math.h"


Body *sun;
Body *following;

/*bool showDeatils = true;
bool showGRange = true;*/

Body *body_new(char *name, Vector pos, Vector v, int r, int mass, char color){
    Body b;
    strcpy(b.name, name);
    b.color = color;
    b.r = r;
    b.mass = mass;
    b.position = pos;
    b.velocity = v;

    return bodyArray_add(b);
}

int body_init() {
    if (bodyArray_init() != 0)
        return 1; // failed to allocate memory for body array

    sun = body_new("Sun", (Vector) {0, 0}, (Vector) {0, 0}, 7, 20, '@');
    if (sun == NULL)
        return 2; // failed to allocate memory for sun

    following = sun;

    return 0;
}

void body_addGravityEffect(Body *dest, Body const *src){
    double d = vector_distance(dest->position, src->position);
    double d2 = d * d;
    double force = src->mass / d2 / targetFPS * 2;

    Vector v = vector_subtract(dest->position, src->position);
    Vector unitVector = vector_scalarDivide(v, d);
    v = vector_invert(vector_scalarMultiply(unitVector, force));

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
    if(infoLayer.enabled) {
        Point p = vector_toPoint(body->position);
        p.y /= 2;
        p = point_subtract(p, screen_offset);
        drawing_drawText(&infoLayer, (int)(p.x - strlen(body->name) / 2), (int)p.y, body->name);
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
                layer_writeAtXY(&bodyLayer, x, y, body->color);
            else if(rangeLayer.enabled && drange < (long long int)(er * 0.8))
                layer_writeAtXY(&rangeLayer, x, y, '.');
        }
    }
    body_drawInfo(body);
}

void body_render(){
    layer_clear(&bodyLayer);
    layer_clear(&rangeLayer);
    layer_clear(&infoLayer);
    for (int i = 0; i < bodyArray.length; ++i)
        body_draw(&bodyArray.data[i]);
}