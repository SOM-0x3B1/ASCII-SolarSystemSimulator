#include "drawing.h"
#include "../global.h"


void drawing_drawText(Layer *l, int x, int y, char const *s, EconioColor color) {
    if(y >= 0 && y < screen_height && x >= 0) {
        for (int i = 0; s[i] != '\0' && x + i < screen_width; ++i) {
            l->text[y][x + i] = s[i];
            l->fgColor[y][x + i] = color;
            layer_writeAtXY(l, x + i, y, s[i], color, color);
        }
    }
}


void drawing_drawLine(Layer *l, int x, int y, int length, bool vertical, char c) {
    if (vertical && y >= 0) {
        for (int i = y; i < y + length && i < screen_height; ++i)
            l->text[i][x] = c;
    } else if (!vertical && x >= 0) {
        for (int i = x; i < x + length && i < screen_width; ++i)
            l->text[y][i] = c;
    }
}