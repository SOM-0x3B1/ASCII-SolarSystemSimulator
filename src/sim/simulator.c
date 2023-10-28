#include <stdlib.h>
#include "simulator.h"
#include "../lib/econio.h"
#include "../global.h"
#include "../gui/edit_menu.h"
#include "body.h"
#include "body_array.h"


void simulation_doVectorCalculations(){
    for (int i = 0; i < bodyArray.length; ++i){
        for (int j = 0; j < bodyArray.length; ++j) {
            if(i != j)
                body_addGravityEffect(&bodyArray.data[i], &bodyArray.data[j]);
        }
    }
}

void simulation_doMovements(){
    for (int i = 0; i < bodyArray.length; ++i)
        body_move(&bodyArray.data[i]);
}

void simulation_tick(){
    if(!pausedByUser) {
        simulation_doVectorCalculations();
        simulation_doMovements();
    }
}

bool simulation_moveCam(EconioKey key){
    if (key == 's' || key == KEY_DOWN) {
        screen_offset.y++;
        return true;
    }
    else if (key == 'w' || key == KEY_UP) {
        screen_offset.y--;
        return true;
    }
    else if (key == 'a' || key == KEY_LEFT) {
        screen_offset.x -= 2;
        return true;
    }
    else if (key == 'd' || key == KEY_RIGHT) {
        screen_offset.x += 2;
        return true;
    } else if(key == ' ') {
        if(pausedByUser)
            pausedByUser = false;
        else
            pausedByUser = true;
    }

    return false;
}

void simulation_processInput() {
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        if (key == KEY_ESCAPE || key == 'e')
            editMenu_switchTo(key);
        else if(simulation_moveCam(key))
            following = NULL;
    }
}