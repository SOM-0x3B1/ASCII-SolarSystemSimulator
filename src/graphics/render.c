#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../lib/econio.h"
#include "render.h"
#include "../global.h"
#include "../sim/body.h"
#include "drawing.h"
#include "../gui/overlay.h"
#include "../gui/edit_menu.h"
#include "../lib/debugmalloc.h"


static char *screenBuffer;
static size_t buffSize;

static time_t frameCountReseted;
static int frameCount = 0;
static int fps = 0;


bool render_init(){
    buffSize = screen_width * screen_height * sizeof(char);
    screenBuffer = (char*) malloc(buffSize);

    frameCountReseted = time(NULL);

    if(screenBuffer != NULL) {
        memset(screenBuffer, '\0', buffSize);
        setvbuf(stdout, screenBuffer, _IOFBF, screen_height * screen_width);
        //setbuf(stdout, NULL); // disable buffering (slow!)
        return true;
    } else
        return false;
}
void render_dispose(){
    free(screenBuffer);
}


void render_handleFPS(){
    // TODO: adaptive simulation speed regulation

    frameCount++;
    if(time(NULL) - frameCountReseted > 0)
    {
        fps = frameCount;
        frameCount = 0;
        frameCountReseted = time(NULL);

        double adjust = 0.001 * ((double)fps / targetFPS);

        if(fps < targetFPS)
            sleepTime -= adjust;
        else if(fps > targetFPS)
            sleepTime += adjust;
    }

    overlay_updateFPS(fps);
}


void render_refreshScreen(){
    for (int y = 0; y < screen_height; ++y) {
        for (int x = 0; x < screen_width; ++x) {
            bool empty = true;
            for (int i = 0; i < layerCount; ++i) {
                Layer *l = layers[i];
                if(l->enabled && l->text[y][x] != '\0') {;
                    fprintf(stdout, "%c", l->text[y][x]);
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
    overlay_render(fps);
    render_refreshScreen();
    if(menuLayer.enabled)
        editMenu_render();
}