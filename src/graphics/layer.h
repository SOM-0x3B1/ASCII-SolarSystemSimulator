#ifndef ASCII_SSS_LAYER_H
#define ASCII_SSS_LAYER_H


#include "../structs.h"
#include <stdbool.h>
#include "../lib/econio.h"
#include "render.h"


/** Initializes the layer arrays. */
bool layer_init(LayerInstances *li, Layer **layers, Screen *screen); //

/** Frees the layer arrays. */
void layer_dispose(Layer **layers);

/**
 * Writes a single character into a layer.
 * @param l Layer
 * @param c character
 */
void layer_writeAtXY(Layer *l, int x, int y, char c, Screen *screen);


/**
 * Replaces every character in all of the layers with \0.
 * @param l Layer
 */
void layer_clearAll(Layer **l, Screen *screen);


#endif //ASCII_SSS_LAYER_H