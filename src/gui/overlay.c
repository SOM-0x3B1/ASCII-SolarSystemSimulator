#include "overlay.h"
#include <stdio.h>
#include "../lib/econio.h"
#include "../graphics/drawing.h"
#include "../global.h"
#include "../graphics/layer.h"


Layer *ol = &overlayLayer;

void overlay_updateFPS(int fps){
    char sfps[16];
    sprintf(sfps, "%d/%d FPS   ", fps, targetFPS);
    drawing_drawText(ol, 2, 0, sfps, COL_WHITE);
}

void overlay_render(int fps) {
    drawing_drawLine(ol, 0, 0, screen_width, false, ' ');
    overlay_updateFPS(fps);
    drawing_drawText(ol, screen_width / 2 - 5, 0, "ASCII-SSS", COL_WHITE);
    drawing_drawText(ol, screen_width - 25, 0, "PRESS 'E' for edit menu", COL_WHITE);
    drawing_drawLine(ol, 0, 1, screen_width, false, '_');

    drawing_drawLine(ol, 0, screen_height - 3, screen_width, false, '_');
    drawing_drawLine(ol, 0, screen_height - 2, screen_width, false, ' ');
    drawing_drawText(ol, 0, screen_height - 2, "Status: RUNNING", COL_WHITE);
    drawing_drawLine(ol, 0, screen_height - 1, screen_width, false, ' ');
    //drawing_drawLine(ol, 0, screen_height - 1, screen_width, false, '_');
}