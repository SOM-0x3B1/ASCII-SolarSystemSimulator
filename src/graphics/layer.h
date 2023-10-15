#ifndef ASCII_SSS_LAYER_H
#define ASCII_SSS_LAYER_H

#endif //ASCII_SSS_LAYER_H

#include <stdbool.h>
#include "../econio/econio.h"


typedef struct Layer{
    char text[30][120];
    EconioColor bgColor[30][120];
    EconioColor fgColor[30][120];
    bool enabled;
} Layer;


extern Layer guiLayer;
extern Layer bodyLayer;
extern Layer trailLayer;
extern Layer menuLayer;


void layer_writeAtXY(Layer l, int x, int y, char c, EconioColor fgc, EconioColor bgc);

void layer_clear(Layer l);