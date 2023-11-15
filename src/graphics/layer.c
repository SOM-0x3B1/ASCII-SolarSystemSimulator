#include "layer.h"
#include "../lib/debugmalloc.h"


bool layer_init(LayerInstances *li, Layer **layers, Screen *screen){
    li->overlayLayer.enabled = true;
    li->menuLayer.enabled = false;
    li->infoLayer.enabled = true;
    li->bodyLayer.enabled = true;
    li->rangeLayer.enabled = true;
    li->trailLayer.enabled = true;

    layers[0] = &li->overlayLayer;
    layers[1] = &li->menuLayer;
    layers[2] = &li->infoLayer;
    layers[3] = &li->bodyLayer;
    layers[4] = &li->rangeLayer;
    layers[5] = &li->trailLayer;

    int screenSize = screen->screen_width * screen->screen_height;
    for (int i = 0; i < LAYER_COUNT; ++i) {
        layers[i]->text = (char **) malloc(screen->screen_height * sizeof(char *));
        layers[i]->text[0] = (char *) malloc(screenSize * sizeof(char));
        for (int y = 1; y < screen->screen_height; ++y)
            layers[i]->text[y] = layers[i]->text[0] + y * screen->screen_width;

        if(layers[i]->text == NULL || layers[i]->text[0] == NULL)
            return false;

        layer_clear(layers[i], screen);
    }

    return true;
}
void layer_dispose(Layer **layers){
    for (int i = 0; i < LAYER_COUNT; ++i) {
        free(layers[i]->text[0]);
        free(layers[i]->text);
    }
}


void layer_writeAtXY(Layer *l, int x, int y, char c, Screen *screen){
    if(x >= 0 && y >= 0 && x < screen->screen_width && y < screen->screen_height)
        l->text[y][x] = c;
}


void layer_clear(Layer *l, Screen *screen) {
    for (int y = 0; y < screen->screen_height; ++y)
        memset(l->text[y], 0, screen->screen_width * sizeof(char));
        /*for (int x = 0; x < screen->screen_width; ++x)
            l->text[y][x] = '\0';*/
}

