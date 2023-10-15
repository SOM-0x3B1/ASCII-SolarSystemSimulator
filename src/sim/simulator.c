#include "simulator.h"
#include "../econio/econio.h"
#include "../graphics/layer.h"
#include "../global.h"


void simulation_doVectorCalculations(){

}

void simulation_doMovements(){

}

void simulation_processInput(){
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        if(key == KEY_ESCAPE || key =='m') {
            currentState = EDIT_MENU;
            menuLayer.enabled = true;
        }
    }
}