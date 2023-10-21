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
    drawing_drawText(ol, 2, 0, sfps);
}

void overlay_render(int fps) {
    drawing_drawLine(ol, 0, 0, screen_width, false, ' ');
    overlay_updateFPS(fps);
    drawing_drawText(ol, screen_width / 2 - 4, 0, "ASCII-SSS");
    drawing_drawText(ol, screen_width - 25, 0, "PRESS 'E' for edit menu");
    drawing_drawLine(ol, 0, 1, screen_width, false, '_');

    drawing_drawLine(ol, 0, screen_height - 3, screen_width, false, '_');
    drawing_drawLine(ol, 0, screen_height - 2, screen_width, false, ' ');
    drawing_drawText(ol, 2, screen_height - 2, "Status: RUNNING");

    char *sCamPos[50];
    sprintf((char*)sCamPos, "Camera center: {%lld ; %lld}", screen_offset.x + screen_width / 2, -screen_offset.y + screen_height);
    drawing_drawText(ol, screen_width / 2 - (int)strlen((char*)sCamPos) / 2, screen_height - 2, (char*)sCamPos);

    char *sFollowing[32];
    sprintf((char*)sFollowing, "Following: %s", (follow == NULL) ? "[FREE]" : follow->name);
    drawing_drawText(ol, screen_width - strlen((char*)sFollowing) - 2, screen_height - 2, (char*)sFollowing);

    drawing_drawLine(ol, 0, screen_height - 1, screen_width, false, ' ');
    //drawing_drawLine(ol, 0, screen_height - 1, screen_width, false, '_');
}