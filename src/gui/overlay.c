#include "overlay.h"
#include <stdio.h>
#include <string.h>
#include "../lib/econio.h"
#include "../graphics/drawing.h"
#include "../global.h"
#include "../sim/body.h"


/** Program states stored as strings. */
static char *sStates[PROGRAM_STATE_COUNT] = {"RUNNING", "RUNNING", "PAUSED (INPUT)", "PAUSED (PLACING)"};


/** Renders the FPS counter. */
static void overlay_writeFPS(){
    char sfps[16];
    sprintf(sfps, "%d/%d FPS   ", fps, targetFPS);
    drawing_drawText(&overlayLayer, 2, 0, sfps);
}


/** Renders the title and the edit menu tips. */
static void overlay_renderHeader(){
    drawing_drawLine(&overlayLayer, 0, 0, screen_width, false, ' ');
    overlay_writeFPS();
    drawing_drawText(&overlayLayer, screen_width / 2 - 4, 0, "ASCII-SSS");
    if(menuLayer.enabled)
        drawing_drawText(&overlayLayer, screen_width - 30, 0, "PRESS 'E' TO CLOSE EDIT MENU");
    else
        drawing_drawText(&overlayLayer, screen_width - 29, 0, "PRESS 'E' TO OPEN EDIT MENU");
    drawing_drawLine(&overlayLayer, 0, 1, screen_width, false, '_');
}


/** Renders the programstate, camera position and the body the camera is potentially following. */
static void overlay_renderFooter(){
    drawing_drawLine(&overlayLayer, 0, screen_height - 3, screen_width, false, '_');
    drawing_drawLine(&overlayLayer, 0, screen_height - 2, screen_width, false, ' ');
    drawing_drawText(&overlayLayer, 2, screen_height - 2, "Status:");
    if(!pausedByUser || (programState != SIMULATION && programState != EDIT_MENU)) {
        if (fullSpeed && programState==SIMULATION)
            drawing_drawText(&overlayLayer, 10, screen_height - 2, "RUNNING (FULL SPEED)");
        else
            drawing_drawText(&overlayLayer, 10, screen_height - 2, sStates[programState]);
    }
    else
        drawing_drawText(&overlayLayer, 10, screen_height - 2, "PAUSED (BY USER)");

    char *sCamPos[50];
    sprintf((char*)sCamPos, "Camera center: {%lld ; %lld}", screen_offset.x + screen_width / 2, -(screen_offset.y + screen_height / 2) * 2);
    drawing_drawText(&overlayLayer, screen_width / 2 - (int)strlen((char*)sCamPos) / 2, screen_height - 2, (char*)sCamPos);

    char *sFollowing[32];
    sprintf((char*)sFollowing, "Following: %s", (following == NULL) ? "[FREE]" : following->name);
    drawing_drawText(&overlayLayer, screen_width - (int)strlen((char*)sFollowing) - 2, screen_height - 2, (char*)sFollowing);

    drawing_drawLine(&overlayLayer, 0, screen_height - 1, screen_width, false, ' ');
}


void overlay_render() {


    overlay_renderHeader();
    overlay_renderFooter();
}