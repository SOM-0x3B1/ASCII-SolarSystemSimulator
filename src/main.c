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

    screen_width = 119;
    screen_height = 29;
    currentState = EDIT_MENU;

    layer_init();
    if (!render_init()){
        // failed to allocate buffer
        exiting = true;
    }

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
        render_fullRender();
        econio_sleep(0.01);
    }

    layer_dispose();
    render_dispose();

    return 0;
}