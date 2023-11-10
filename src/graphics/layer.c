#include "layer.h"
#include "../global.h"
#include "../lib/debugmalloc.h"


Layer overlayLayer;
Layer menuLayer;
Layer infoLayer;
Layer bodyLayer;
Layer rangeLayer;
Layer trailLayer;

Layer *layers[LAYER_COUNT];


bool layer_init(){
    overlayLayer.enabled = true;
    menuLayer.enabled = false;
    infoLayer.enabled = true;
    bodyLayer.enabled = true;
    rangeLayer.enabled = true;
    trailLayer.enabled = true;

    layers[0] = &overlayLayer;
    layers[1] = &menuLayer;
    layers[2] = &infoLayer;
    layers[3] = &bodyLayer;
    layers[4] = &rangeLayer;
    layers[5] = &trailLayer;

    int screenSize = screen_width * screen_height;
    for (int i = 0; i < LAYER_COUNT; ++i) {
        layers[i]->text = (char **) malloc(screen_height * sizeof(char *));
        layers[i]->text[0] = (char *) malloc(screenSize * sizeof(char));
        for (int y = 1; y < screen_height; ++y)
            layers[i]->text[y] = layers[i]->text[0] + y * screen_width;

        if(layers[i]->text == NULL || layers[i]->text[0] == NULL)
            return false;

        layer_clear(layers[i]);
    }

    return true;
}
void layer_dispose(){
    for (int i = 0; i < LAYER_COUNT; ++i) {
        free(layers[i]->text[0]);
        free(layers[i]->text);
    }
}


void layer_writeAtXY(Layer *l, int x, int y, char c){
    if(x >= 0 && y >= 0 && x < screen_width && y < screen_height)
        l->text[y][x] = c;
}


void layer_clear(Layer *l){
    for (int y = 0; y < screen_height; ++y) {
        for (int x = 0; x < screen_width; ++x)
            l->text[y][x] = '\0';
    }
}

