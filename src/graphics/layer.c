#include "layer.h"
#include "string.h"


/** Replaces every character in a layers with \0. */
static void layer_clear(Layer *l, Screen *screen);


Error layer_init(LayerInstances *li, Layer **layers, Screen *screen) {
    li->overlayLayer.enabled = false;
    li->menuLayer.enabled = true;
    li->infoLayer.enabled = false;
    li->bodyLayer.enabled = false;
    li->rangeLayer.enabled = false;
    li->trailLayer.enabled = false;

    layers[0] = &li->overlayLayer;
    layers[1] = &li->menuLayer;
    layers[2] = &li->infoLayer;
    layers[3] = &li->bodyLayer;
    layers[4] = &li->rangeLayer;
    layers[5] = &li->trailLayer;

    for (int i = 0; i < LAYER_COUNT; ++i) {
        layers[i]->text = (char **) malloc(screen->height * sizeof(char *));
        layers[i]->text[0] = (char *) malloc(screen->bufferSize);
        for (int y = 1; y < screen->height; ++y)
            layers[i]->text[y] = layers[i]->text[0] + y * screen->width;

        if (layers[i]->text == NULL || layers[i]->text[0] == NULL)
            return ERR_MEMORY;

        layer_clear(layers[i], screen);
    }

    return SUCCESS;
}

void layer_dispose(Layer **layers) {
    for (int i = 0; i < LAYER_COUNT; ++i) {
        free(layers[i]->text[0]);
        free(layers[i]->text);
    }
}


void layer_writeAtXY(Layer *l, int x, int y, char c, Screen *screen) {
    if (x >= 0 && y >= 0 && x < screen->width && y < screen->height)
        l->text[y][x] = c;
}


static void layer_clear(Layer *l, Screen *screen) {
    memset(l->text[0], 0, screen->bufferSize);
}


void layer_clearAll(Layer **layers, Screen *screen) {
    for (int i = 0; i < LAYER_COUNT; ++i)
        if (layers[i]->enabled)
            layer_clear(layers[i], screen);
}
