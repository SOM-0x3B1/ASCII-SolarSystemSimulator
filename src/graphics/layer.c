#include <stdlib.h>
#include "layer.h"
#include "../global.h"
#include "../lib/debugmalloc.h"


Layer overlayLayer;
Layer menuLayer;
Layer infoLayer;
Layer bodyLayer;
Layer trailLayer;

const int layerCount = LAYER_COUNT;
Layer *layers[LAYER_COUNT];

bool layer_init(){
    overlayLayer.enabled = true;
    menuLayer.enabled = false;
    infoLayer.enabled = true;
    bodyLayer.enabled = true;
    trailLayer.enabled = true;

    layers[0] = &overlayLayer;
    layers[1] = &menuLayer;
    layers[2] = &infoLayer;
    layers[3] = &bodyLayer;
    layers[4] = &trailLayer;

    int screenSize = screen_width * screen_height;
    for (int i = 0; i < layerCount; ++i) {
        layers[i]->text = (char **) malloc(screen_height * sizeof(char *));
        layers[i]->text[0] = (char *) malloc(screenSize * sizeof(char));
        for (int y = 1; y < screen_height; ++y)
            layers[i]->text[y] = layers[i]->text[0] + y * screen_width;

        if(layers[i]->text == NULL || layers[i]->text[0] == NULL)
            return false;


        layers[i]->fgColor = (EconioColor **) malloc(screen_height * sizeof(EconioColor *));
        layers[i]->fgColor[0] = (EconioColor *) malloc(screenSize * sizeof(EconioColor));
        //layers[i]->bgColor[0] = (EconioColor *) malloc(screenSize * sizeof(EconioColor));
        for (int y = 1; y < screen_height; ++y)
            layers[i]->fgColor[y] = layers[i]->fgColor[0] + y * screen_width;

        if(layers[i]->fgColor == NULL || layers[i]->fgColor[0] == NULL)
            return false;

        //layers[i]->bgColor = (EconioColor **) malloc(screenSize * sizeof(EconioColor *));
        layer_clear(layers[i]);
    }

    return true;
}

void layer_dispose(){
    for (int i = 0; i < layerCount; ++i) {
        free(layers[i]->text[0]);
        free(layers[i]->text);
        free(layers[i]->fgColor[0]);
        free(layers[i]->fgColor);
        /*free(layers[i]->bgColor[0]);
        free(layers[i]->bgColor);*/
    }
}

void layer_writeAtXY(Layer *l, int x, int y, char c, EconioColor fgc, EconioColor bgc){
    l->text[y][x] = c;
    l->fgColor[y][x] = fgc;
    //l->bgColor[y][x] = bgc;
}

void layer_clear(Layer *l){
    for (int y = 0; y < screen_height; ++y) {
        for (int x = 0; x < screen_width; ++x)
            l->text[y][x] = '\0';
    }
}

