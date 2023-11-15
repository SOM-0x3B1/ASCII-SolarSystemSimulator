#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../lib/econio.h"
#include "render.h"
#include "../sim/body.h"
#include "drawing.h"
#include "../gui/overlay.h"
#include "../gui/edit_menu.h"
#include "../lib/debugmalloc.h"
#include "../gui/body_editor.h"
#include "../fs.h"


bool render_init(Screen *screen) {
    size_t buffSize = screen->screen_width * screen->screen_height * sizeof(char);
    screen->screenBuffer = (char *) malloc(buffSize);

    screen->frameCountResetedTime = time(NULL);

    if (screen->screenBuffer != NULL) {
        memset(screen->screenBuffer, '\0', buffSize);
        if (setvbuf(stdout, screen->screenBuffer, _IOFBF, screen->screen_height * screen->screen_width))
            return false;
    } else
        return false;

    return true;
}
void render_dispose(Screen *screen){
    free(screen->screenBuffer);
}


void render_resetFPSMeasurement(Screen *screen){
    screen->fps = screen->targetFPS;
    screen->frameCount = 0;
    screen->frameCountResetedTime = time(NULL);
}

/** Updates the current FPS value and regulates simulation speed. */
static void render_handleFPS(Program *program, Simulation *sim, Screen *screen) {
    screen->frameCount++;
    if (time(NULL) - screen->frameCountResetedTime > 0) {
        screen->fps = screen->frameCount;
        screen->frameCount = 0;
        screen->frameCountResetedTime = time(NULL);

        if(sim->pausedByUser || !sim->fullSpeed) {
            double adjustment = 0.001 * ((double) screen->fps / screen->targetFPS);
            if (screen->fps < screen->targetFPS)
                program->sleepTime -= adjustment;
            else if (screen->fps > screen->targetFPS)
                program->sleepTime += adjustment;
        }
    }
}


void render_refreshScreen(Program *program, Simulation *sim, Screen *screen, LayerProperties *lp){
    for (int y = 0; y < screen->screen_height; ++y) {
        for (int x = 0; x < screen->screen_width; ++x) {
            bool empty = true;
            for (int i = 0; i < LAYER_COUNT; ++i) {
                if(lp->layers[i]->enabled && lp->layers[i]->text[y][x] != '\0') {
                    fprintf(stdout, "%c", lp->layers[i]->text[y][x]);
                    empty = false;
                    break;
                }
            }
            if(empty)
                fprintf(stdout, " ");
        }
        if(y < screen->screen_height - 1)
            fprintf(stdout, "\n");
    }

    econio_flush();
    econio_gotoxy(0,0);

    render_handleFPS(program, sim, screen);
}


void render_fullRender(Program *program, Simulation *sim, Screen *screen, LayerProperties *lp, GUI *gui){
    body_render(&lp->layerInstances, sim, screen);
    overlay_render(program, sim, screen, &lp->layerInstances);
    if(lp->layerInstances.menuLayer.enabled)
        editMenu_render(&lp->layerInstances, screen, gui, sim);

    if(program->textInputDest == TEXT_INPUT_BODY_EDITOR && (program->programState == TEXT_INPUT || program->programState == PLACING_BODY))
        bodyEditor_render(program, &lp->layerInstances, screen, gui);
    else if(program->textInputDest == TEXT_INPUT_EXPORT && program->programState == TEXT_INPUT)
        export_render(gui, &lp->layerInstances, screen);

    render_refreshScreen(program, sim, screen, lp);
}