#ifndef ASCII_SSS_SIMULATOR_H
#define ASCII_SSS_SIMULATOR_H


#include "../structs.h"
#include <stdlib.h>
#include "body_array.h"
#include "../graphics/render.h"


/** Performs all of the simulation steps. */
void simulation_tick(Simulation *sim, Screen *screen);

/** Processes the raw input. */
void simulation_processInput(Simulation *sim, Screen *screen, Program *program, Gui *gui, LayerInstances *li);


#endif //ASCII_SSS_SIMULATOR_H