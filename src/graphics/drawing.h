#ifndef ASCII_SSS_DRAWING_H
#define ASCII_SSS_DRAWING_H


#include "layer.h"
#include "../econio/econio.h"


void drawing_drawText(Layer *l, char const *s, int x, int y, EconioColor color);

void drawing_drawLine(Layer *l, int x, int y, int h, bool vertical);


#endif //ASCII_SSS_DRAWING_H
