#include "layer.h"
#include "../global.h"


Layer guiLayer;
Layer bodyLayer;
Layer trailLayer;
Layer menuLayer;


void layer_writeAtXY(Layer l, int x, int y, char c, EconioColor fgc, EconioColor bgc){
    l.text[y][x] = c;
    l.bgColor[y][x] = fgc;
    l.bgColor[y][x] = bgc;
}

void layer_clear(Layer l){
    for (int y = 0; y < screen_height; ++y) {
        for (int x = 0; x < screen_width; ++x)
            l.text[y][x] = ' ';
    }
}