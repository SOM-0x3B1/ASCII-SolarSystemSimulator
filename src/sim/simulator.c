#include "simulator.h"
#include "../econio/econio.h"
#include "../graphics/layer.h"
#include "../global.h"
#include "../gui/edit_menu.h"


void simulation_doVectorCalculations(){

}

void simulation_doMovements(){

}

void simulation_processInput(){
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        if(key == KEY_ESCAPE || key =='m')
            editMenu_switchTo();
    }
}