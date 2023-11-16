#include <stdlib.h>
#include "simulator.h"
#include "../gui/edit_menu.h"
#include "body.h"


/** Processes the gravitatinal interactions of the bodies, and updates their velocity. */
static void simulation_doGravityCalculations(BodyArray *ba, Simulation *sim){
    for (int i = 0; i < ba->length; ++i){
        for (int j = 0; j < ba->length; ++j) {
            if(i != j)
                body_addGravityEffect(&ba->data[i], &ba->data[j], sim);
        }
    }
}


/** Moves the bodies. */
static void simulation_doMovements(Simulation *sim, Screen *screen){
    for (int i = 0; i < sim->bodyArray.length; ++i) {
        body_move(& sim->bodyArray.data[i]);
        if (sim->trailSpacingCounter > screen->targetFPS / 2)
            trail_enqueue(& sim->bodyArray.data[i].trail,  sim->bodyArray.data[i].position);
    }
    if (sim->trailSpacingCounter > screen->targetFPS / 2)
        sim->trailSpacingCounter = 0;
    sim->trailSpacingCounter++;
}


/** Checks if any of the bodies is colliding with another body.  */
static void simulation_detectCollisions(BodyArray *ba, Simulation *sim){
    for (int i = 0; i < ba->length - 1; ++i) {
        for (int j = i + 1; j < ba->length; ++j) {
            Body *a = &ba->data[i];
            Body *b = &ba->data[j];
            body_detectCollision(a, b, sim);
        }
    }
}


void simulation_tick(Simulation *sim, Screen *screen){
    if(!sim->pausedByUser) {
        simulation_doGravityCalculations(&sim->bodyArray, sim);
        simulation_doMovements(sim, screen);
        simulation_detectCollisions(&sim->bodyArray, sim);
    }
}


/**
 * Moves the camera in accordance to the input key.
 * @return Has the camare been moved
 */
static bool simulation_moveCam(EconioKey key, Screen *screen){
    if (key == 's' || key == KEY_DOWN) {
        screen->offset.y++;
        return true;
    }
    else if (key == 'w' || key == KEY_UP) {
        screen->offset.y--;
        return true;
    }
    else if (key == 'a' || key == KEY_LEFT) {
        screen->offset.x -= 2;
        return true;
    }
    else if (key == 'd' || key == KEY_RIGHT) {
        screen->offset.x += 2;
        return true;
    }
    return false;
}


void simulation_processInput(Simulation *sim, Screen *screen, Program *program, Gui *gui, LayerInstances *li) {
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        if (key == KEY_ESCAPE || key == 'e')
            editMenu_switchTo(key, program, gui, screen, li);
        else if (key == ' ') {
            sim->pausedByUser = !sim->pausedByUser;
        } else if (key == 'q') {
            sim->fullSpeed = !sim->fullSpeed;
            if (!sim->fullSpeed)
                render_resetFPSMeasurement(screen);
        } else if (simulation_moveCam(key, screen))
            sim->following = NULL;
    }
}