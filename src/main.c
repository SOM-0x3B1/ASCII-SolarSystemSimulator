#include <stdio.h>
#include <stdbool.h>
#include "structs.h"
#include "lib/econio.h"
#include "graphics/render.h"
#include "sim/simulator.h"
#include "graphics/layer.h"
#include "gui/edit_menu.h"
#include "sim/body.h"
#include "sim/body_array.h"
#include "lib/debugmalloc.h"
#include "gui/body_editor.h"
#include "fs.h"
#include "gui/error.h"


/** Initialize modules with dinamic memory management. */
void init_modulesWithDMM(bool *exiting, Program *program, LayerStatic *ls, Simulation *sim, Screen *screen);

/** Frees allocated memory, and clears the screen. */
void exitProgram(LayerStatic *ls, Simulation sim, Screen *screen);


int main() {
    econio_set_title("ASCII Solar System Simulator");
    econio_rawmode();

    // Default settings
    Program program;
    program.sleepTime = 0.02;
    program.error = SUCCESS;
    program.exiting = false;
    program.state = PROGRAM_STATE_SIMULATION;

    Screen screen;
    screen.width = 119;
    screen.height = 30;
    screen.offset = (Point){0, 0};
    screen.fps = 0;
    screen.targetFPS = 30;
    screen.frameCount = 0; // Frames since last reset (~1s)

    Simulation sim;
    sim.solarMass = 333000;
    sim.detectCollisionPercentage = 0.8;
    sim.fullSpeed = false;
    sim.pausedByUser = false;
    sim.trailSpacingCounter = 0;

    Gui gui;
    gui.editMenu_state = EDIT_MENU_STATE_MAIN;
    gui.cursorPos = 0;

    LayerStatic layerStatic;


    // Attept to load settings.ini
    program.error = settings_loadSettings(&sim, &screen);


    screen.bufferSize = screen.height * screen.width * sizeof(char);

    init_modulesWithDMM(&program.exiting, &program, &layerStatic, &sim, &screen);


    // Adds some default bodies to the sim
    body_new("Mars", (Vector) {-50, 0}, (Vector) {0, -0.15}, 3, 0.15, '#', &sim);
    body_new("Venus", (Vector) {13, 0}, (Vector) {0, 0.3}, 4, 0.815, '#', &sim);
    body_new("Earth", (Vector) {30, 0}, (Vector) {0, 0.2}, 5, 1, '#', &sim);


    // Main program loop
    while (!program.exiting){
        if(program.error == SUCCESS) {
            switch (program.state) {
                case PROGRAM_STATE_EDIT_MENU:
                    simulation_tick(&sim, &screen);
                    program.error = editMenu_processInput(&program, &sim, &screen, &gui, &layerStatic.layerInstances);
                    break;
                case PROGRAM_STATE_SIMULATION:
                    simulation_tick(&sim, &screen);
                    simulation_processInput(&sim, &screen, &program, &gui, &layerStatic.layerInstances);
                    break;
                case PROGRAM_STATE_PLACING_BODY:
                    bodyEditor_processPlacementInput(&program, &gui, &sim);
                    break;
                case PROGRAM_STATE_TEXT_INPUT:
                    // empty, because input processing will occur after render
                    break;
            }

            render_fullRender(&program, &sim, &screen, &layerStatic, &gui);

            // Speed & FPS regulator
            if (sim.pausedByUser || !sim.fullSpeed)
                econio_sleep(program.sleepTime);

            // For scanf inputs
            if (program.state == PROGRAM_STATE_TEXT_INPUT) {
                switch (program.textInputDest) {
                    case TEXT_INPUT_BODY_EDITOR:
                        program.error = bodyEditor_processTextInput(&program, &gui, &sim);
                        break;
                    case TEXT_INPUT_EXPORT:
                        program.error = export_processTextInput(&gui, &program, &sim);
                        break;
                    case TEXT_INPUT_IMPORT:
                        break;
                }
            }
        }

        // Handle error
        if (program.error != SUCCESS){
            error_render(program.error, &screen, &layerStatic.layerInstances);
            render_refreshScreen(&program, &sim, &screen, &layerStatic);

            error_awaitConfirmation();

            if (program.error == ERR_MEMORY)
                program.exiting = true;
            else
                program.error = SUCCESS;
        }
    }


    exitProgram(&layerStatic, sim, &screen);

    return 0;
}


void init_modulesWithDMM(bool *exiting, Program *program, LayerStatic *ls, Simulation *sim, Screen *screen) {
    if (layer_init(&ls->layerInstances, ls->layers, screen) != SUCCESS ||
        render_init(screen) != SUCCESS ||
        body_init(sim) != SUCCESS) {
        program->error = ERR_MEMORY;
    }
}


void exitProgram(LayerStatic *ls, Simulation sim, Screen *screen){
    econio_clrscr();
    econio_gotoxy(0, 0);
    printf("Exiting...\n\n");

    layer_dispose(ls->layers);
    render_dispose(screen);
    bodyArray_dispose(&sim.bodyArray);

    debugmalloc_atexit_dump();

    econio_sleep(5);
}