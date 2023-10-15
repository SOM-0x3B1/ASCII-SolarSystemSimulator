#include <stdio.h>
#include <stdbool.h>
#include "econio/econio.h"
#include "global.h"
#include "graphics/render.h"
#include "sim/simulator.h"
#include "graphics/layer.h"


int main() {
    econio_set_title("ASCII Solar System Simulator");
    econio_rawmode();

    screen_width = 120;
    screen_height = 30;
    currentState = EDIT_MENU;

    guiLayer.enabled = true;
    bodyLayer.enabled = true;
    trailLayer.enabled = true;
    menuLayer.enabled = true;

    while (!exiting){
        switch (currentState) {
            case EDIT_MENU:
                break;
            case SIMULATION:
                simulation_doVectorCalculations();
                simulation_doMovements();
                simulation_processInput();
                break;
        }
        econio_sleep(0.1);
    }

    return 0;
}