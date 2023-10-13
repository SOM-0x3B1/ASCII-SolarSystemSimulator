#include "layer.h"

void layer_writeXY(Layer l, int x, int y, char c, EconioColor fgc, EconioColor bgc){
    l.text[y][x] = c;
    l.bgColor[y][x] = fgc;
    l.bgColor[y][x] = bgc;
}

void layer_clear(Layer l){
    for (int y = 0; y < 30; ++y) {
        for (int x = 0; x < 120; ++x)
            l.text[y][x] = ' ';
    }
}