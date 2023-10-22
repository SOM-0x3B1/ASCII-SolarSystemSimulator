#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "lib/econio.h"
#include "global.h"
#include "graphics/render.h"
#include "sim/simulator.h"
#include "graphics/layer.h"
#include "gui/edit_menu.h"
#include "sim/body.h"
#include "sim/body_array.h"
#include "lib/debugmalloc.h"


int getINIParam(char* dest, const char* src);

int getIntValue(const char* src, int start, int *value);

int loadSettings();

void exitProgram();


int main() {
    econio_set_title("ASCII Solar System Simulator");
    econio_rawmode();

    int loadSettingResult = loadSettings();
    if(loadSettingResult != 0) {
        // ERR: unable to load settings from settings.ini
        screen_width = 119;
        screen_height = 30;
        targetFPS = 30;
    }

    currentState = SIMULATION;


    if(!layer_init()){
        // ERR: failed to allocate layer buffer(s)
        exiting = true;
    }
    if (!render_init()){
        // ERR: failed to allocate screen buffer
        exiting = true;
    }
    if(body_init())
    {   // ERR: failed to allocate body array
        exiting = true;
    }

    body_new("Earth", (Vector) {40, 0}, (Vector) {0, 0.15}, 4, 5, '#');

    while (!exiting){
        switch (currentState) {
            case EDIT_MENU:
                simulation_tick();
                editMenu_processInput();
                break;
            case SIMULATION:
                simulation_tick();
                simulation_processInput();
                break;
        }
        render_fullRender();
        econio_sleep(sleepTime);
    }

    exitProgram();

    return 0;
}



int getINIParam(char* dest, const char* src) {
    int i;
    for (i = 0; src[i] != '='; ++i)
        dest[i] = src[i];
    dest[i] = '\0';

    return i;
}

int getIntValue(const char* src, int start, int *value) {
    *value = 0;
    int i;
    for (i = start + 1; src[i] != '\0'; ++i) {
        if(src[i] < '0' || src[i] > '9')
            return 1;
        *value = *value * 10 + (src[i] - '0');
    }
    return 0;
}

int loadSettings() {
    FILE *f;
    f = fopen("settings.ini", "r");

    if(f != NULL) {
        char line[32];
        while (fscanf(f, "%s", line) != EOF) {
            char param[16];
            int valueStart = getINIParam(param, line);

            int value;
            if (getIntValue(line, valueStart, &value) != 0)
                return 3; // invalid value

            if (strcmp(param, "screen_width") == 0)
                screen_width = value;
            else if (strcmp(param, "screen_height") == 0)
                screen_height = value;
            else if (strcmp(param, "targetFPS") == 0) {
                if(value > 0)
                    targetFPS = value;
                else
                    return 4; // target fps must be higher than 0
            }
            else
                return 2; // invalid parameter
        }
        fclose(f);
    } else
        return 1; // unable to open

    return 0;
}

void exitProgram(){
    econio_clrscr();
    econio_gotoxy(0, 0);
    printf("Exiting...\n\n");

    layer_dispose();
    render_dispose();
    bodyArray_dispose();

    debugmalloc_atexit_dump();

    econio_sleep(5);
}