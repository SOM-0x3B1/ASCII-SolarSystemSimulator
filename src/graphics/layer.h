#ifndef ASCII_SSS_LAYER_H
#define ASCII_SSS_LAYER_H

#include <stdbool.h>
#include "../lib/econio.h"

#define LAYER_COUNT 6


/** A 2D matrix that can be written onto the console. */
typedef struct Layer{
    char **text; // 2D array
    bool enabled;
} Layer;


extern Layer overlayLayer;
extern Layer menuLayer;
extern Layer infoLayer;
extern Layer bodyLayer;
extern Layer rangeLayer;
extern Layer trailLayer;

extern Layer *layers[LAYER_COUNT]; // The array of layers in order of priority (0. > 1. > ...).

/** Initializes the layer arrays. */
bool layer_init(); //

/** Frees the layer arrays. */
void layer_dispose();

/**
 * Writes a single character into a layer.
 * @param l Layer
 * @param c character
 */
void layer_writeAtXY(Layer *l, int x, int y, char c);

/**
 * Replaces every character in the layer with \0-s
 * @param l Layer
 */
void layer_clear(Layer *l);


#endif //ASCII_SSS_LAYER_H