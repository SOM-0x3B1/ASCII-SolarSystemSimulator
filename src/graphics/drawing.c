#include "drawing.h"
#include "../global.h"


void drawing_drawText(Layer *l, int x, int y, char const *s) {
    for (int i = 0; s[i] != '\0' && x + i < screen_width; ++i)
        layer_writeAtXY(l, x + i, y, s[i]);
}


void drawing_drawLine(Layer *l, int x, int y, int length, bool vertical, char c) {
    if (vertical) {
        for (int i = y; i <= y + length && i < screen_height; ++i)
            layer_writeAtXY(l, x, i, c);
    }
    else {
        for (int i = x; i <= x + length && i < screen_width; ++i)
            layer_writeAtXY(l, i, y, c);
    }
}

void swapInts(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void drawing_drawRectangle(Layer *l, int x1, int y1, int x2, int y2, char c){
    if(x1 > x2)
        swapInts(&x1, &x2);
    if(y1 > y2)
        swapInts(&y1, &y2);
    for (int y = y1; y <= y2 && y < screen_height; ++y) {
        for (int x = x1; x <= x2 && x < screen_width; ++x)
            layer_writeAtXY(l, x, y, c);
    }
}