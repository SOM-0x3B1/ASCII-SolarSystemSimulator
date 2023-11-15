#include <string.h>
#include "drawing.h"
#include "../gui/edit_menu.h"


static void swapInts(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void sortPoints(int x1, int y1, int x2, int y2){
    if(x1 > x2)
        swapInts(&x1, &x2);
    if(y1 > y2)
        swapInts(&y1, &y2);
}


void drawing_drawText(Layer *l, int x, int y, const char *s, Screen *screen) {
    for (int i = 0; s[i] != '\0' && x + i < screen->screen_width; ++i)
        layer_writeAtXY(l, x + i, y, s[i], screen);
}


void drawing_drawLine(Layer *l, int x, int y, int length, bool vertical, char c, Screen *screen) {
    if (vertical) {
        for (int i = y; i <= y + length && i < screen->screen_height; ++i)
            layer_writeAtXY(l, x, i, c, screen);
    }
    else {
        for (int i = x; i <= x + length && i < screen->screen_width; ++i)
            layer_writeAtXY(l, i, y, c, screen);
    }
}


void drawing_drawRectangle(Layer *l, int x1, int y1, int x2, int y2, char c, Screen *screen){
    sortPoints(x1, y1, x2, y2);
    for (int y = y1; y <= y2 && y < screen->screen_height; ++y) {
        for (int x = x1; x <= x2 && x < screen->screen_width; ++x)
            layer_writeAtXY(l, x, y, c, screen);
    }
}


int drawing_drawBox(Layer *l, int x1, int y1, int x2, int y2, const char* title, Screen *screen){
    sortPoints(x1, y1, x2, y2);

    int xCenter = x1 + (x2 - x1) / 2;

    drawing_drawRectangle(l, x1 + 1, y1 + 1, x2 - 1, y2 - 1, ' ', screen);

    drawing_drawLine(l, x1 + 1, y1, x2-x1 - 2, false, '_', screen);
    drawing_drawText(l, xCenter - (int)strlen(title) / 2, y1 + 1, title, screen);
    drawing_drawLine(l, x1 + 1, y1 + 2, x2 - x1 -2, false, '_', screen);
    drawing_drawLine(l, x1, y2, x2- x1, false, '_', screen);

    drawing_drawLine(l, x1, y1 + 1, y2 - y1 - 1, true, '|', screen);
    drawing_drawLine(l, x2, y1 + 1, y2 - y1 - 1, true, '|', screen);

    return xCenter;
}


Point drawing_drawInputPrompt(Layer *l, int yCenter, const char *title, const char *text, Screen *screen){
    Point textPos;
    int margin = screen->screen_width / 4;
    int xCentrer = drawing_drawBox(l, margin - EDIT_MENU_WIDTH / 2, yCenter - 4,
                                   screen->screen_width - margin - EDIT_MENU_WIDTH / 2, yCenter + 4,
                                   title, screen);

    drawing_drawText(l, margin - EDIT_MENU_WIDTH / 2 + 3, yCenter + 1, text, screen);
    textPos = (Point){margin + 3 - EDIT_MENU_WIDTH / 2 + (int)strlen(text) + 2, yCenter + 1};

    drawing_drawText(l, xCentrer - 12, yCenter + 3, "Press 'ENTER' to accept", screen);

    return textPos;
}