#include <stdio.h>
#include <stdbool.h>
#include "lib/econio.h"
#include "global.h"
#include "graphics/render.h"
#include "sim/simulator.h"
#include "graphics/layer.h"
#include "gui/edit_menu.h"
#include "gui/overlay.h"
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
    bodyEditor_init();
    editMenu_init();
    overlay_init();


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

        econio_sleep(sleepTime);

        if(programState == TEXT_INPUT)
            bodyEditor_processTextInput();
    }

    exitProgram();

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