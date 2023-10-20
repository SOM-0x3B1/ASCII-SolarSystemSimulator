#ifndef ASCII_SSS_DRAWING_H
#define ASCII_SSS_DRAWING_H


#include "layer.h"
#include "../lib/econio.h"


void drawing_drawText(Layer *l, int x, int y, char const *s, EconioColor color);

void drawing_drawLine(Layer *l, int x, int y, int length, bool vertical, char c);


#endif //ASCII_SSS_DRAWING_H
