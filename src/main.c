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
    render_init();

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
        render_full();
        econio_sleep(0.01);
    }

    return 0;
}