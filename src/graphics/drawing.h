#ifndef ASCII_SSS_DRAWING_H
#define ASCII_SSS_DRAWING_H


#include "layer.h"
#include "../lib/econio.h"
#include "../vector.h"


void drawing_drawText(Layer *l, int x, int y, char const *s);

void drawing_drawLine(Layer *l, int x, int y, int length, bool vertical, char c);

void drawing_drawRectangle(Layer *l, int x1, int y1, int x2, int y2, char c);

int drawing_drawBox(Layer *l, int x1, int y1, int x2, int y2, const char* title);

Point drawing_drawInputPrompt(Layer *l, int yCenter, const char *title, const char *text);


#endif //ASCII_SSS_DRAWING_H
