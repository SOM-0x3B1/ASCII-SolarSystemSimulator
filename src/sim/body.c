#include <stdlib.h>
#include <stdio.h>
#include "body.h"
#include "../global.h"
#include <string.h>
#include "../graphics/drawing.h"
#include "math.h"

#define TRAIL_LENGTH 15


Body *sun;
Body *following;
Body *editedBody;

double solarMass;

double detectCollisionPercentage;


int trail_spacing_counter = 0;

/*bool showDeatils = true;
bool showGRange = true;*/


Body *body_new(char *name, Vector pos, Vector v, double r, double mass, char color){
    Body b;
    strcpy(b.name, name);
    b.color = color;
    b.r = r;
    b.mass = mass;
    b.position = pos;
    b.velocity = v;

    b.trail = (TrailQueue*) malloc(sizeof(TrailQueue));
    trailQueue_init(b.trail, &b);

    return bodyArray_add(&b);
}

int body_init() {
    if (bodyArray_init() != 0)
        return 1; // failed to allocate memory for body array

    sun = body_new("Sun", (Vector) {0, 0}, (Vector) {0, 0}, 7, solarMass, '@');
    if (sun == NULL)
        return 2; // failed to allocate memory for sun

    following = sun;

    return 0;
}


void body_addGravityEffect(Body *dest, Body const *src){
    double d = vector_distance(dest->position, src->position);
    double d2 = d * d * solarMass;
    double force = src->mass / d2; // TODO: adaptive simulation speed regulation

    Vector v = vector_subtract(dest->position, src->position);
    Vector unitVector = vector_scalarDivide(v, d);
    v = vector_invert(vector_scalarMultiply(unitVector, force));

    dest->velocity = vector_add(dest->velocity, v);
}


void trailQueue_init(TrailQueue *tq, Body *b) {
    tq->head = (Trail *) malloc(sizeof(Trail));
    tq->head->position = vector_toPoint(b->position);
    tq->head->next = NULL;

    tq->capacity = TRAIL_LENGTH;
    tq->length = 1;
}

Trail *trail_dequeue(TrailQueue *tq){
    Trail *ct = tq->head;
    for (int i = 0; i < tq->length - 2; ++i)
        ct = ct->next;

    Trail *res = ct->next;
    free(ct->next);
    ct->next = NULL;
    tq->length--;
    return res;
}

void trail_enqueue(TrailQueue *tq, Vector v) {
    Point p = vector_toPoint(v);
    Trail *newT = (Trail *) malloc(sizeof(Trail));
    newT->position = p;
    newT->next = tq->head;
    tq->head = newT;
    tq->length++;

    if(tq->length > tq->capacity)
        trail_dequeue(tq);
}

void trailQueue_clear(TrailQueue *tq){
    for (int i = 0; i < tq->length; ++i)
        trail_dequeue(tq);
}


void body_move(Body *body){
    body->position = vector_add(body->position, body->velocity); // TODO: adaptive simulation speed regulation
}


static void swapBodyPointers(Body **a, Body **b){
    Body *temp = *a;
    *a = *b;
    *b = temp;
}

static void collide(Body *a, Body *b){
    if (b->mass > a->mass)
        swapBodyPointers(&a, &b);
    if(following == b)
        following = a;
    a->mass += b->mass;
    a->r = sqrt((a->r * a->r) + (b->r + b->r) * 3.14);
    a->velocity = vector_add(a->velocity, vector_scalarMultiply(b->velocity, b->mass / a->mass));
    bodyArray_remove(b);
}

void body_detectCollision(Body *a, Body *b) {
    double d = vector_distance(a->position, b->position);
    if (d < (a->r + b->r) * detectCollisionPercentage)
        collide(a, b);
}


void body_drawInfo(Body const *body) {
    if(infoLayer.enabled) {
        Point p = vector_toPoint(body->position);
        p.y /= 2;
        p = point_subtract(p, screen_offset);
        drawing_drawText(&infoLayer, (int)(p.x - strlen(body->name) / 2), (int)p.y, body->name);
    }
}

void body_drawTrail(Body const *body) {
    Trail *t = body->trail->head;
    int i = 1;
    do {
        Point p = t->position;
        p.y /= 2;
        p = point_subtract(p, screen_offset);

        char c = i < TRAIL_LENGTH / 1.5 ? '+' : '.';
        i++;

        layer_writeAtXY(&trailLayer, (int) p.x, (int) p.y, c);
        t = t->next;
    } while (t != NULL);
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
            double er = sqrt(body->mass * 100);

            long long int drange = llabs((dx2dy2 / 2) - (long long int)(er * er));

            if (dx2dy2 <= (int)(body->r * body->r))
                layer_writeAtXY(&bodyLayer, x, y, body->color);
            else if(rangeLayer.enabled && drange < (long long int)(er * 0.8))
                layer_writeAtXY(&rangeLayer, x, y, '.');
        }
    }
    body_drawInfo(body);
    body_drawTrail(body);
}

void body_render(){
    layer_clear(&bodyLayer);
    layer_clear(&rangeLayer);
    layer_clear(&infoLayer);
    layer_clear(&trailLayer);

    for (int i = 0; i < bodyArray.length; ++i) {
        Body *b = &bodyArray.data[i];

        if(following == &bodyArray.data[i]) {
            Point p = vector_toPoint(b->position);
            p.y /= 2;
            Point screenSize = {screen_width / 2, screen_height / 2};
            p = point_subtract(p, screenSize);
            if(menuLayer.enabled)
                p.x += 16;
            screen_offset = p;
        }

        body_draw(b);
    }
}