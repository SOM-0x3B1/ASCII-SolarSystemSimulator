#include <stdlib.h>
#include "body.h"
#include <string.h>
#include "../graphics/drawing.h"
#include "math.h"
#include "../lib/debugmalloc.h"


#define TRAIL_LENGTH 15


Body *body_new(char *name, Vector pos, Vector v, double r, double mass, char color, Simulation *sim){
    Body b;
    strcpy(b.name, name);
    b.color = color;
    b.r = r;
    b.mass = mass;
    b.position = pos;
    b.velocity = v;

    b.trail = (TrailQueue){};
    trailQueue_init(&b.trail, &b);

    return bodyArray_add(&sim->bodyArray, &b, sim);
}


int body_init(Simulation *sim) {
    if (bodyArray_init(&sim->bodyArray) != 0)
        return 1; // failed to allocate memory for body array

    sim->sun = body_new("Sun", (Vector) {0, 0}, (Vector) {0, 0}, 7, sim->solarMass, '@', sim);
    if (sim->sun == NULL)
        return 2; // failed to allocate memory for sun

    sim->following = sim->sun;

    return 0;
}


void body_addGravityEffect(Body *dest, Body const *src, Simulation *sim){
    double d = vector_distance(dest->position, src->position);
    double d2 = d * d * sim->solarMass;
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
    Trail *cNode = tq->head;
    while (cNode != NULL){
        Trail *next = cNode->next;
        free(cNode);
        cNode = next;
    }
}


void body_move(Body *body){
    body->position = vector_add(body->position, body->velocity); // TODO: adaptive simulation speed regulation
}


static void swapBodyPointers(Body **a, Body **b){
    Body *temp = *a;
    *a = *b;
    *b = temp;
}

/**
 *  Collides a lighter body into a heavier one.
 *  The order of parameters is arbitrary.
 */
static void collide(Body *a, Body *b, Simulation *sim){
    if (b->mass > a->mass)
        swapBodyPointers(&a, &b);
    if(sim->following == b)
        sim->following = a;
    if(sim->editedBody == b)
        sim->editedBody = a;
    a->mass += b->mass;
    a->r = sqrt((a->r * a->r) + (b->r + b->r) * 3.14);
    a->velocity = vector_add(a->velocity, vector_scalarMultiply(b->velocity, b->mass / a->mass));
    bodyArray_remove(&sim->bodyArray, b, sim);
}


void body_detectCollision(Body *a, Body *b, Simulation *sim) {
    if(sim->detectCollisionPercentage > 0) {
        double d = vector_distance(a->position, b->position);
        if (d < (a->r + b->r) * sim->detectCollisionPercentage)
            collide(a, b, sim);
    }
}


/** Draws the info part of a body. */
static void body_drawInfo(Body const *body, LayerInstances *li, Screen *screen) {
    if(li->infoLayer.enabled) {
        Point p = vector_toPoint(body->position);
        p.y /= 2;
        p = point_subtract(p, screen->offset);
        drawing_drawText(&li->infoLayer, (int)(p.x - strlen(body->name) / 2), (int)p.y, body->name, screen);
    }
}


/** Draws the trail part of a body. */
static void body_drawTrail(Body const *body, LayerInstances *li, Screen *screen) {
    Trail *t = body->trail.head;
    int i = 1;
    do {
        Point p = t->position;
        p.y /= 2;
        p = point_subtract(p, screen->offset);

        char c = i < TRAIL_LENGTH / 1.5 ? '+' : '.';
        i++;

        layer_writeAtXY(&li->trailLayer, (int) p.x, (int) p.y, c, screen);
        t = t->next;
    } while (t != NULL);
}


/** Draws the body part of a body. */
static void body_draw(Body const *body, Simulation *sim, LayerInstances *li, Screen *screen){
    Point p = vector_toPoint(body->position);
    p.y /= 2;
    p = point_subtract(p, screen->offset);

    Point pLight;
    bool drawShadows = !li->infoLayer.enabled && body != sim->sun;
    if(drawShadows) {
        pLight = vector_toPoint(body->position);
        Point pLightUnit = vector_toPoint(vector_unitVector(body->position, sim->sun->position));
        pLight = point_subtract(pLight, point_scalarMultiply(pLightUnit, (int) body->r / 2));
        pLight.y /= 2;
        pLight = point_subtract(pLight, screen->offset);
    }

    for (int y = 0; y < screen->height; y++) {
        for (int x = 0; x < screen->width; x++) {
            long long int dX = x - p.x;
            long long int dY = (y - p.y) * 2;
            long long int dx2dy2 = (dX * dX) + (dY * dY);

            long long int dx2dy2Light;
            if(drawShadows) {
                long long int ldX = x - pLight.x;
                long long int ldY = (y - pLight.y) * 2;
                dx2dy2Light = (ldX * ldX) + (ldY * ldY);
            }

            double er = sqrt(body->mass * 100);

            long long int drange = llabs((dx2dy2 / 2) - (long long int)(er * er));

            if (dx2dy2 <= (int)(body->r * body->r)) {
                if(!drawShadows || dx2dy2Light <= (int)(body->r * body->r))
                    layer_writeAtXY(&li->bodyLayer, x, y, body->color, screen);
                else
                    layer_writeAtXY(&li->bodyLayer, x, y, ':', screen);
            }
            else if(li->rangeLayer.enabled && drange < (long long int)(er * 0.8))
                layer_writeAtXY(&li->rangeLayer, x, y, '.', screen);
        }
    }
    body_drawInfo(body, li, screen);
    body_drawTrail(body, li, screen);
}


/** Sets the camera position so the followed body is in the center of the screen. */
static void moveCameraToBody(Body *b, Screen *screen, LayerInstances *li){
    Point p = vector_toPoint(b->position);
    p.y /= 2;
    Point screenSize = {screen->width / 2, screen->height / 2};
    p = point_subtract(p, screenSize);
    if(li->menuLayer.enabled)
        p.x += 16;
    screen->offset = p;
}


void body_render(LayerInstances *li, Simulation *sim, Screen *screen){
    for (int i = 0; i < sim->bodyArray.length; ++i) {
        Body *b = &sim->bodyArray.data[i];

        if(sim->following == &sim->bodyArray.data[i])
            moveCameraToBody(&sim->bodyArray.data[i], screen, li);

        body_draw(b, sim, li, screen);
    }
}