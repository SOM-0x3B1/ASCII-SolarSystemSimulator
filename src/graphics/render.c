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


char *screenBuffer;
size_t buffSize;

time_t frameCountReseted;
int frameCount = 0;
int fps = 0;

Body testBody;


bool render_init(){
    buffSize = screen_width * screen_height * sizeof(char);
    screenBuffer = (char*) malloc(buffSize);

    if(screenBuffer != NULL) {
        memset(screenBuffer, '\0', buffSize);

        testBody.color = COL_LIGHTYELLOW;
        testBody.size = 6;
        Vector pos = {20, 30};
        testBody.position = pos;
        return true;
    } else
        return false;

    frameCountReseted = time(NULL);
}
void render_dispose(){
    free(screenBuffer);
}

void render_refreshScreen(){
    char sfps[10];
    sprintf(sfps, "%d FPS", fps);
    drawing_drawText(&guiLayer, sfps, 2, 1, COL_WHITE);

    for (int y = 0; y < screen_height; ++y) {
        for (int x = 0; x < screen_width; ++x) {
            bool empty = true;
            for (int i = 0; i < layerCount; ++i) {
                Layer *l = layers[i];
                if(l->enabled && l->text[y][x] != ' ') {
                    //EconioColor bgColor = l->bgColor[y][x];
                    EconioColor fgColor = l->fgColor[y][x];
                    econio_textcolor(fgColor);
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
    setvbuf(stdout, screenBuffer, _IOFBF, screen_height * screen_width);

    econio_gotoxy(0,0);
    econio_flush();

    frameCount++;
    if(time(NULL) - frameCountReseted)
    {
        fps = frameCount;
        frameCount = 0;
        frameCountReseted = time(NULL);

        if(fps < 60)
            sleepTime /= 1.1;
        else if(fps > 60)
            sleepTime *= 1.1;
    }
}

void render_renderBodies(){
    layer_clear(&bodyLayer);

    Vector v = {0.01, -0.01};
    testBody.position = vector_add(testBody.position, v);

    body_draw(&testBody);
}

void render_fullRender(){
    render_renderBodies();
    render_refreshScreen();
}