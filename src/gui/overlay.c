#include "overlay.h"
#include <stdio.h>
#include "../econio/econio.h"
#include "../graphics/drawing.h"
#include "../global.h"
#include "../graphics/layer.h"


Layer *ol = &overlayLayer;

void overlay_updateFPS(int fps){
    char sfps[16];
    sprintf(sfps, "%d FPS   ", fps);
    drawing_drawText(ol, sfps, 2, 0, COL_WHITE);
}

void overlay_render(int fps){
    overlay_updateFPS(fps);
    drawing_drawText(ol, "ASCII-SSS", screen_width / 2 - 17, 0, COL_WHITE);
    drawing_drawText(ol, "PRESS 'M' for edit menu", screen_width - 25, 0, COL_WHITE);
    drawing_drawLine(ol, 0, 1, screen_width, false);

    drawing_drawLine(ol, 0, screen_height-3, screen_width, false);
    drawing_drawText(ol,"Status: RUNNING", 0 , screen_height-2, COL_WHITE);
    drawing_drawLine(ol, 0, screen_height-1, screen_width, false);
}