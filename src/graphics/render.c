#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../econio/econio.h"
#include "render.h"
#include "../global.h"
#include "../sim/body.h"


char buff[120*30];

void render_init(){
    memset(buff, '\0', sizeof(buff));
}

void render_refreshScreen(){
    for (int y = 0; y < screen_height; ++y) {
        for (int x = 0; x < screen_width; ++x) {
            bool empty = true;
            for (int i = 0; i < layerCount; ++i) {
                Layer *l = layers[i];
                if(l->enabled && l->text[y][x] != ' ') {
                    EconioColor bgColor = l->bgColor[y][x];
                    EconioColor fgColor = l->fgColor[y][x];
                    econio_textcolor(fgColor);
                    fprintf(stdout, "%c", l->text[y][x]);
                    empty = false;
                    break;
                }
            }
            if(empty) {
                econio_textbackground(COL_BLACK);
                fprintf(stdout, " ");
            }
        }
        fprintf(stdout, "\n");
    }
    setvbuf(stdout, buff, _IOFBF, 120*30);

    econio_gotoxy(0,0);
    econio_flush();
}

void render_bodies(){
    layer_clear(&bodyLayer);

    Body testBody;
    testBody.color = COL_RED;
    testBody.size = 6;
    Vector pos = {20, 15};
    testBody.position = pos;

    body_draw(testBody);
}

void render_full(){
    render_bodies();
    render_refreshScreen();
}