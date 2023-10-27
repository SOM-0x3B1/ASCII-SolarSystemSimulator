#include <stdio.h>
#include <stdbool.h>
#include "lib/econio.h"
#include "global.h"
#include "graphics/render.h"
#include "sim/simulator.h"
#include "graphics/layer.h"
#include "gui/edit_menu.h"
#include "sim/body.h"
#include "sim/body_array.h"
#include "lib/debugmalloc.h"
#include "gui/body_editor.h"
#include "file_manager.h"


void exitProgram();


int main() {
    econio_set_title("ASCII Solar System Simulator");
    econio_rawmode();

    screen_width = 119;
    screen_height = 30;
    targetFPS = 30;
    solarMass = 333000;
    useLegacyRendering = false;

    int loadSettingResult = settings_loadSettings();
    if(loadSettingResult != 0) {
        // ERR: unable to load settings from settings.ini
    }

    programState = SIMULATION;


    if(!layer_init()){
        // ERR: failed to allocate layer buffer(s)
        exiting = true;
    }
    if (!render_init()){
        // ERR: failed to allocate screen buffer
        exiting = true;
    }
    if(body_init() != 0)
    {   // ERR: failed to allocate body array
        exiting = true;
    }

    following = body_new("Mars", (Vector) {-50, 0}, (Vector) {0, -0.15}, 3, 0.15, '+');
    following = body_new("Venus", (Vector) {13, 0}, (Vector) {0, 0.3}, 4, 0.815, '#');
    following = body_new("Earth", (Vector) {30, 0}, (Vector) {0, 0.2}, 4, 1, '#');


    while (!exiting){
        switch (programState) {
            case EDIT_MENU:
                simulation_tick();
                editMenu_processInput();
                break;
            case SIMULATION:
                simulation_tick();
                simulation_processInput();
                break;
            case PLACING_BODY:
                bodyEditor_processPlacementInput();
                break;
            case TEXT_INPUT:
                break;
        }
        render_fullRender();

        if(!useLegacyRendering)
            econio_sleep(sleepTime);

        if(programState == TEXT_INPUT)
            bodyEditor_processTextInput();
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
            else if (strcmp(param, "solarMass") == 0) {
                if(value > 0)
                    solarMass = value;
                else
                    return 5; // sunmass must be higher than 0
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