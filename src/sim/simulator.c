#include "simulator.h"
#include "../lib/econio.h"
#include "../graphics/layer.h"
#include "../global.h"
#include "../gui/edit_menu.h"
#include "body.h"


void simulation_doVectorCalculations(){

}

void simulation_doMovements(){
    body_move(&sun);
}

void simulation_tick(){
    simulation_doVectorCalculations();
    simulation_doMovements();
}

void simulation_processInput() {
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        if (key == KEY_ESCAPE || key == 'e')
            editMenu_switchTo(key);
        else if (key == 's' || key == KEY_DOWN)
            screen_offset.y++;
        else if (key == 'w' || key == KEY_UP)
            screen_offset.y--;
        else if (key == 'a' || key == KEY_LEFT)
            screen_offset.x -= 2;
        else if (key == 'd' || key == KEY_RIGHT)
            screen_offset.x += 2;
    }
}