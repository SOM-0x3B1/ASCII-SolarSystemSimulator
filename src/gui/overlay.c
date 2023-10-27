#include "overlay.h"
#include <stdio.h>
#include <string.h>
#include "../lib/econio.h"
#include "../graphics/drawing.h"
#include "../global.h"
#include "../graphics/layer.h"
#include "../sim/body.h"


static Layer *ol = &overlayLayer;

static char *sStates[4] = {"RUNNING", "RUNNING", "PAUSED (INPUT)", "PAUSED (PLACING)"};


void overlay_updateFPS(int fps){
    char sfps[16];
    sprintf(sfps, "%d/%d FPS   ", fps, targetFPS);
    drawing_drawText(ol, 2, 0, sfps);
}

void overlay_render(int fps) {
    drawing_drawLine(ol, 0, 0, screen_width, false, ' ');
    overlay_updateFPS(fps);
    drawing_drawText(ol, screen_width / 2 - 4, 0, "ASCII-SSS");
    if(menuLayer.enabled)
        drawing_drawText(ol, screen_width - 30, 0, "PRESS 'E' TO CLOSE EDIT MENU");
    else
        drawing_drawText(ol, screen_width - 29, 0, "PRESS 'E' TO OPEN EDIT MENU");
    drawing_drawLine(ol, 0, 1, screen_width, false, '_');

    drawing_drawLine(ol, 0, screen_height - 3, screen_width, false, '_');
    drawing_drawLine(ol, 0, screen_height - 2, screen_width, false, ' ');
    drawing_drawText(ol, 2, screen_height - 2, "Status:");
    drawing_drawText(ol, 10, screen_height - 2, sStates[programState]);

    char *sCamPos[50];
    sprintf((char*)sCamPos, "Camera center: {%lld ; %lld}", screen_offset.x + screen_width / 2, -(screen_offset.y + screen_height / 2) * 2);
    drawing_drawText(ol, screen_width / 2 - (int)strlen((char*)sCamPos) / 2, screen_height - 2, (char*)sCamPos);

    char *sFollowing[32];
    sprintf((char*)sFollowing, "Following: %s", (following == NULL) ? "[FREE]" : following->name);
    drawing_drawText(ol, screen_width - strlen((char*)sFollowing) - 2, screen_height - 2, (char*)sFollowing);

    drawing_drawLine(ol, 0, screen_height - 1, screen_width, false, ' ');
    //drawing_drawLine(ol, 0, screen_height - 1, screen_width, false, '_');
}