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
    for (int i = 0; i < bodyArray.length; ++i) {
        body_move(&bodyArray.data[i]);
        if (trail_spacing_counter > targetFPS / 2)
            trail_enqueue(&bodyArray.data[i].trail, bodyArray.data[i].position);
    }
    if (trail_spacing_counter > targetFPS / 2)
        trail_spacing_counter = 0;
    trail_spacing_counter++;
}


void simulation_detectCollisions(){
    for (int i = 0; i < bodyArray.length - 1; ++i) {
        for (int j = i + 1; j < bodyArray.length; ++j) {
            Body *a = &bodyArray.data[i];
            Body *b = &bodyArray.data[j];
            body_detectCollision(a, b);
        }
    }
}

void simulation_tick(){
    if(!pausedByUser) {
        simulation_doVectorCalculations();
        simulation_doMovements();
        simulation_detectCollisions();
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
        else if(key == ' ') {
            if(pausedByUser)
                pausedByUser = false;
            else
                pausedByUser = true;
        } else if(key == 'q')
            fullSpeed = !fullSpeed;
        else if(simulation_moveCam(key))
            following = NULL;
    }
}