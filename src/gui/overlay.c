#include "overlay.h"
#include <stdio.h>
#include <string.h>


/** Renders the FPS counter. */
static void overlay_writeFPS(Screen *screen, LayerInstances *li){
    char sfps[16];
    sprintf(sfps, "%d/%d FPS   ", screen->fps, screen->targetFPS);
    drawing_drawText(&li->overlayLayer, 2, 0, sfps, screen);
}


/** Renders the title and the edit menu tips. */
static void overlay_renderHeader(Screen *screen, LayerInstances *li){
    drawing_drawLine(&li->overlayLayer, 0, 0, screen->width, false, ' ', screen);
    overlay_writeFPS(screen, li);
    drawing_drawText(&li->overlayLayer, screen->width / 2 - 4, 0, "ASCII-SSS", screen);
    if(li->menuLayer.enabled)
        drawing_drawText(&li->overlayLayer, screen->width - 30, 0, "PRESS 'E' TO CLOSE EDIT MENU", screen);
    else
        drawing_drawText(&li->overlayLayer, screen->width - 29, 0, "PRESS 'E' TO OPEN EDIT MENU", screen);
    drawing_drawLine(&li->overlayLayer, 0, 1, screen->width, false, '_', screen);
}


/** Returns a string describing the current state of the program. */
static char *programStateToString(ProgramState s){
    switch (s) {
        case PROGRAM_STATE_SIMULATION:
        case PROGRAM_STATE_EDIT_MENU:
            return "RUNNING";
        case PROGRAM_STATE_TEXT_INPUT:
            return "PAUSED (INPUT)";
        case PROGRAM_STATE_PLACING_BODY:
            return "PAUSED (PLACING)";
        case PROGRAM_STATE_MAIN_MENU:
            return "PAUSED (MAIN)";
    }
}


/** Renders the programstate, camera position and the body the camera is potentially following. */
static void overlay_renderFooter(Program *program, Screen *screen, LayerInstances *li, Simulation *sim){
    drawing_drawLine(&li->overlayLayer, 0, screen->height - 3, screen->width, false, '_', screen);
    drawing_drawLine(&li->overlayLayer, 0, screen->height - 2, screen->width, false, ' ', screen);
    drawing_drawText(&li->overlayLayer, 2, screen->height - 2, "Status:", screen);
    if(!sim->pausedByUser || (program->state != PROGRAM_STATE_SIMULATION && program->state != PROGRAM_STATE_EDIT_MENU)) {
        if (sim->fullSpeed && program->state == PROGRAM_STATE_SIMULATION)
            drawing_drawText(&li->overlayLayer, 10, screen->height - 2, "RUNNING (FULL SPEED)", screen);
        else
            drawing_drawText(&li->overlayLayer, 10, screen->height - 2, programStateToString(program->state), screen);
    }
    else
        drawing_drawText(&li->overlayLayer, 10, screen->height - 2, "PAUSED (BY USER)", screen);

    char *sCamPos[50];
    sprintf((char*)sCamPos, "Camera center: {%lld ; %lld}", screen->offset.x + screen->width / 2, -(screen->offset.y + screen->height / 2) * 2);
    drawing_drawText(&li->overlayLayer, screen->width / 2 - (int)strlen((char*)sCamPos) / 2, screen->height - 2, (char*)sCamPos, screen);

    char *sFollowing[32];
    sprintf((char*)sFollowing, "Following: %s", (sim->following == NULL) ? "[FREE]" : sim->following->name);
    drawing_drawText(&li->overlayLayer, screen->width - (int)strlen((char*)sFollowing) - 2, screen->height - 2, (char*)sFollowing, screen);

    drawing_drawLine(&li->overlayLayer, 0, screen->height - 1, screen->width, false, ' ', screen);
}


void overlay_render(Program *program, Simulation *sim, Screen *screen, LayerInstances *li) {
    overlay_renderHeader(screen, li);
    overlay_renderFooter(program, screen, li, sim);
}