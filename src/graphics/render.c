#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../lib/econio.h"
#include "render.h"
#include "../global.h"
#include "../sim/body.h"
#include "drawing.h"
#include "../gui/overlay.h"
#include "../gui/edit_menu.h"
#include "../lib/debugmalloc.h"
#include "../gui/body_editor.h"
#include "../fs.h"


static char *screenBuffer; // Used as the buffer of the console

static time_t frameCountResetedTime; // The last time the FPS was evaluated
static int frameCount = 0; // Frames since last reset (~1s)


bool render_init() {
    size_t buffSize = screen_width * screen_height * sizeof(char);
    screenBuffer = (char *) malloc(buffSize);

    frameCountResetedTime = time(NULL);

    if (screenBuffer != NULL) {
        memset(screenBuffer, '\0', buffSize);
        if (setvbuf(stdout, screenBuffer, _IOFBF, screen_height * screen_width))
            return false;
    } else
        return false;

    return true;
}
void render_dispose(){
    free(screenBuffer);
}


void render_resetFPSMeasurement(){
    fps = targetFPS;
    frameCount = 0;
    frameCountResetedTime = time(NULL);
}

/** Updates the current FPS value and regulates simulation speed. */
static void render_handleFPS() {
    frameCount++;
    if (time(NULL) - frameCountResetedTime > 0) {
        fps = frameCount;
        frameCount = 0;
        frameCountResetedTime = time(NULL);

        if(pausedByUser || !fullSpeed) {
            double adjustment = 0.001 * ((double) fps / targetFPS);
            if (fps < targetFPS)
                sleepTime -= adjustment;
            else if (fps > targetFPS)
                sleepTime += adjustment;
        }
    }
}


void render_refreshScreen(){
    for (int y = 0; y < screen_height; ++y) {
        for (int x = 0; x < screen_width; ++x) {
            bool empty = true;
            for (int i = 0; i < LAYER_COUNT; ++i) {
                if(layers[i]->enabled && layers[i]->text[y][x] != '\0') {
                    fprintf(stdout, "%c", layers[i]->text[y][x]);
                    empty = false;
                    break;
                }
            }
            if(empty)
                fprintf(stdout, " ");
        }
        if(y < screen_height - 1)
            fprintf(stdout, "\n");
    }

    econio_flush();
    econio_gotoxy(0,0);

    render_handleFPS();
}


void render_fullRender(){
    body_render();
    overlay_render();
    if(menuLayer.enabled)
        editMenu_render();

    if(textInputDest == TEXT_INPUT_BODY_EDITOR && (programState == TEXT_INPUT || programState == PLACING_BODY))
        bodyEditor_render();
    else if(textInputDest == TEXT_INPUT_EXPORT && programState == TEXT_INPUT)
        export_render();

    render_refreshScreen();
}