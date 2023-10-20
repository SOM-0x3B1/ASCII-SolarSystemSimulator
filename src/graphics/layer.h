#ifndef ASCII_SSS_LAYER_H
#define ASCII_SSS_LAYER_H

#include <stdbool.h>
#include "../lib/econio.h"

#define LAYER_COUNT 5


typedef struct Layer{
    char **text;
    EconioColor **bgColor;
    EconioColor **fgColor;
    bool enabled;
} Layer;


extern Layer overlayLayer;
extern Layer menuLayer;
extern Layer infoLayer;
extern Layer bodyLayer;
extern Layer trailLayer;

extern const int layerCount;
extern Layer *layers[LAYER_COUNT];


bool layer_init();
void layer_dispose();

void layer_writeAtXY(Layer *l, int x, int y, char c, EconioColor fgc, EconioColor bgc);

void layer_clear(Layer *l);


#endif //ASCII_SSS_LAYER_H