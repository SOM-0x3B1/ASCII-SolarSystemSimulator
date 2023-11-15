#include "overlay.h"
#include <stdio.h>
#include <string.h>
#include "../lib/econio.h"



/** Renders the FPS counter. */
static void overlay_writeFPS(Screen *screen, LayerInstances *li){
    char sfps[16];
    sprintf(sfps, "%d/%d FPS   ", screen->fps, screen->targetFPS);
    drawing_drawText(&li->overlayLayer, 2, 0, sfps, screen);
}


/** Renders the title and the edit menu tips. */
static void overlay_renderHeader(Screen *screen, LayerInstances *li){
    drawing_drawLine(&li->overlayLayer, 0, 0, screen->screen_width, false, ' ', screen);
    overlay_writeFPS(screen, li);
    drawing_drawText(&li->overlayLayer, screen->screen_width / 2 - 4, 0, "ASCII-SSS", screen);
    if(li->menuLayer.enabled)
        drawing_drawText(&li->overlayLayer, screen->screen_width - 30, 0, "PRESS 'E' TO CLOSE EDIT MENU", screen);
    else
        drawing_drawText(&li->overlayLayer, screen->screen_width - 29, 0, "PRESS 'E' TO OPEN EDIT MENU", screen);
    drawing_drawLine(&li->overlayLayer, 0, 1, screen->screen_width, false, '_', screen);
}


static char *programStateToString(ProgramState s){
    switch (s) {
        case SIMULATION:
        case EDIT_MENU:
            return "RUNNING";
        case TEXT_INPUT:
            return "PAUSED (INPUT)";
        case PLACING_BODY:
            return "PAUSED (PLACING)";
    }
}


/** Renders the programstate, camera position and the body the camera is potentially following. */
static void overlay_renderFooter(Program *program, Screen *screen, LayerInstances *li, Simulation *sim){
    drawing_drawLine(&li->overlayLayer, 0, screen->screen_height - 3, screen->screen_width, false, '_', screen);
    drawing_drawLine(&li->overlayLayer, 0, screen->screen_height - 2, screen->screen_width, false, ' ', screen);
    drawing_drawText(&li->overlayLayer, 2, screen->screen_height - 2, "Status:", screen);
    if(!sim->pausedByUser || (program->programState != SIMULATION && program->programState != EDIT_MENU)) {
        if (sim->fullSpeed && program->programState==SIMULATION)
            drawing_drawText(&li->overlayLayer, 10, screen->screen_height - 2, "RUNNING (FULL SPEED)", screen);
        else
            drawing_drawText(&li->overlayLayer, 10, screen->screen_height - 2, programStateToString(program->programState), screen);
    }
    else
        drawing_drawText(&li->overlayLayer, 10, screen->screen_height - 2, "PAUSED (BY USER)", screen);

    char *sCamPos[50];
    sprintf((char*)sCamPos, "Camera center: {%lld ; %lld}", screen->screen_offset.x + screen->screen_width / 2, -(screen->screen_offset.y + screen->screen_height / 2) * 2);
    drawing_drawText(&li->overlayLayer, screen->screen_width / 2 - (int)strlen((char*)sCamPos) / 2, screen->screen_height - 2, (char*)sCamPos, screen);

    char *sFollowing[32];
    sprintf((char*)sFollowing, "Following: %s", (sim->following == NULL) ? "[FREE]" : sim->following->name);
    drawing_drawText(&li->overlayLayer, screen->screen_width - (int)strlen((char*)sFollowing) - 2, screen->screen_height - 2, (char*)sFollowing, screen);

    drawing_drawLine(&li->overlayLayer, 0, screen->screen_height - 1, screen->screen_width, false, ' ', screen);
}


void overlay_render(Program *program, Simulation *sim, Screen *screen, LayerInstances *li) {
    overlay_renderHeader(screen, li);
    overlay_renderFooter(program, screen, li, sim);
}