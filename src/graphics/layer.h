#ifndef ASCII_SSS_LAYER_H
#define ASCII_SSS_LAYER_H


#include <stdbool.h>
#include "../econio/econio.h"


typedef struct Layer{
    char **text;
    EconioColor **bgColor;
    EconioColor **fgColor;
    bool enabled;
} Layer;


extern Layer guiLayer;
extern Layer menuLayer;
extern Layer bodyLayer;
extern Layer trailLayer;

extern const int layerCount;
extern Layer *layers[4];


bool layer_init();
void layer_dispose();

void layer_writeAtXY(Layer *l, int x, int y, char c, EconioColor fgc, EconioColor bgc);

void layer_clear(Layer *l);


#endif //ASCII_SSS_LAYER_H