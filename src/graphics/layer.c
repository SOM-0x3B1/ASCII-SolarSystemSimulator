#include "layer.h"
#include "../global.h"


Layer guiLayer;
Layer menuLayer;
Layer bodyLayer;
Layer trailLayer;

int layerCount = 4;
Layer *layers[4];


void layer_init(){
    guiLayer.enabled = true;
    menuLayer.enabled = true;
    bodyLayer.enabled = true;
    trailLayer.enabled = true;

    layers[0] = &guiLayer;
    layers[1] = &menuLayer;
    layers[2] = &bodyLayer;
    layers[3] = &trailLayer;

    for (int i = 0; i < 4; ++i)
        layer_clear(layers[i]);
}

void layer_writeAtXY(Layer *l, int x, int y, char c, EconioColor fgc, EconioColor bgc){
    l->text[y][x] = c;
    l->fgColor[y][x] = fgc;
    l->bgColor[y][x] = bgc;
}

void layer_clear(Layer *l){
    for (int y = 0; y < screen_height; ++y) {
        for (int x = 0; x < screen_width; ++x)
            l->text[y][x] = ' ';
    }
}