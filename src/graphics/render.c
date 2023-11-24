#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../lib/econio.h"
#include "render.h"
#include "../gui/overlay.h"
#include "../gui/edit_menu.h"
#include "../lib/debugmalloc.h"
#include "../gui/body_editor.h"
#include "../fs.h"


Error render_init(Screen *screen) {
    size_t buffSize = screen->width * screen->height * sizeof(char);
    screen->buffer = (char *) malloc(buffSize);

    screen->frameCountResetedTime = time(NULL);

    if (screen->buffer != NULL) {
        memset(screen->buffer, '\0', buffSize);
        if (setvbuf(stdout, screen->buffer, _IOFBF, screen->height * screen->width) != 0)
            return ERR_MEMORY;
    } else
        return ERR_MEMORY;

    return SUCCESS;
}
void render_dispose(Screen *screen){
    free(screen->buffer);
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


void render_refreshScreen(Program *program, Simulation *sim, Screen *screen, LayerStatic *ls){
    econio_gotoxy(0,0);

    for (int y = 0; y < screen->height; ++y) {
        for (int x = 0; x < screen->width; ++x) {
            bool empty = true;
            for (int i = 0; i < LAYER_COUNT; ++i) {
                if(ls->layers[i]->enabled && ls->layers[i]->text[y][x] != '\0') {
                    fprintf(stdout, "%c", ls->layers[i]->text[y][x]);
                    empty = false;
                    break;
                }
            }
            if(empty)
                fprintf(stdout, " ");
        }
        if(y < screen->height - 1)
            fprintf(stdout, "\n");
    }

    econio_flush();
    econio_gotoxy(0,0);

    render_handleFPS(program, sim, screen);
}


void render_fullRender(Program *program, Simulation *sim, Screen *screen, LayerStatic *ls, Gui *gui){
    layer_clearAll(ls->layers, screen);

    body_render(&ls->layerInstances, sim, screen);
    overlay_render(program, sim, screen, &ls->layerInstances);
    if(ls->layerInstances.menuLayer.enabled)
        editMenu_render(&ls->layerInstances, screen, gui, sim);

    if(program->textInputDest == TEXT_INPUT_BODY_EDITOR && (program->state == PROGRAM_STATE_TEXT_INPUT || program->state == PROGRAM_STATE_PLACING_BODY))
        bodyEditor_render(program, &ls->layerInstances, screen, gui);
    else if(program->textInputDest == TEXT_INPUT_EXPORT && program->state == PROGRAM_STATE_TEXT_INPUT)
        fs_export_render(gui, &ls->layerInstances, screen);

    render_refreshScreen(program, sim, screen, ls);
}