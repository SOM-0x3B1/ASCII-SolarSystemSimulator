#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../econio/econio.h"
#include "render.h"
#include "../global.h"
#include "../sim/body.h"
#include "drawing.h"
#include "../gui/overlay.h"
#include "../gui/edit_menu.h"


char *screenBuffer;
size_t buffSize;

time_t frameCountReseted;
int frameCount = 0;
int fps = 0;


bool render_init(){
    buffSize = screen_width * screen_height * sizeof(char);
    screenBuffer = (char*) malloc(buffSize);
    //setvbuf(stdout, NULL, _IONBF, 0);

    frameCountReseted = time(NULL);

    if(screenBuffer != NULL) {
        memset(screenBuffer, '\0', buffSize);
        setvbuf(stdout, screenBuffer, _IOFBF, screen_height * screen_width);
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

        if(fps < targetFPS)
            sleepTime *= 0.95;
        else if(fps > targetFPS)
            sleepTime *= 1.05;
    }

    overlay_updateFPS(fps);
}


void render_refreshScreen(){
    for (int y = 0; y < screen_height; ++y) {
        for (int x = 0; x < screen_width; ++x) {
            bool empty = true;
            for (int i = 0; i < layerCount; ++i) {
                Layer *l = layers[i];
                if(l->enabled && l->text[y][x] != '\0') {
                    //EconioColor bgColor = l->bgColor[y][x];
                    EconioColor fgColor = l->fgColor[y][x];
                    //econio_textcolor(fgColor);
                    fprintf(stdout, "%c", l->text[y][x]);
                    empty = false;
                    break;
                }
            }
            if(empty) {
                //econio_textbackground(COL_BLACK);
                fprintf(stdout, " ");
            }
        }
        fprintf(stdout, "\n");
    }
    //setbuf(stdout, NULL);
    //setvbuf(stdout, NULL, _IONBF, 0);

    econio_flush();
    econio_gotoxy(0,0);

    render_handleFPS();
}


/*void render_renderBodies(){
    layer_clear(&bodyLayer);
    body_render();
}*/


void render_fullRender(){
    //render_renderBodies();
    body_render();
    overlay_render(fps);
    render_refreshScreen();
    if(menuLayer.enabled)
        editMenu_render();
}