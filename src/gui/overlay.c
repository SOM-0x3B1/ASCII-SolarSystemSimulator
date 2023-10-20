#include "overlay.h"
#include <stdio.h>
#include <string.h>
#include "../lib/econio.h"
#include "../graphics/drawing.h"
#include "../global.h"
#include "../graphics/layer.h"
#include "../sim/body.h"


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
    drawing_drawText(ol, 2, screen_height - 2, "Status: RUNNING", COL_WHITE);

    char *sCamPos[32];
    sprintf((char*)sCamPos, "Camera center: {%d; %d}", screen_offset.x + screen_width / 2, -screen_offset.y + screen_height);
    drawing_drawText(ol, screen_width / 2 - strlen((char*)sCamPos) / 2, screen_height - 2, (char*)sCamPos, COL_WHITE);

    char *sFollowing[32];
    sprintf((char*)sFollowing, "Following: %s", (follow == NULL) ? "[FREE]" : follow->name);
    drawing_drawText(ol, screen_width - strlen((char*)sFollowing) - 2, screen_height - 2, (char*)sFollowing, COL_WHITE);

    drawing_drawLine(ol, 0, screen_height - 1, screen_width, false, ' ');
    //drawing_drawLine(ol, 0, screen_height - 1, screen_width, false, '_');
}