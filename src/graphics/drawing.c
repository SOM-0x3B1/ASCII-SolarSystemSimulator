#include "drawing.h"
#include "../global.h"


void drawing_drawText(Layer *l, int x, int y, char const *s, EconioColor color) {
    for (int i = 0; s[i] != '\0'; ++i) {
        l->text[y][x + i] = s[i];
        l->fgColor[y][x + i] = color;
        layer_writeAtXY(l, x+i, y, s[i], color, color);
    }
}


void drawing_drawLine(Layer *l, int x, int y, int h, bool vertical, char c) {
    if (vertical && y >= 0) {
        for (int i = y; i < y + h && i < screen_height; ++i)
            l->text[i][x] = c;
    } else if (!vertical && x >= 0) {
        for (int i = x; i < x + h && i < screen_width; ++i)
            l->text[y][i] = c;
    }
}