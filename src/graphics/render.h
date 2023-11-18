#ifndef ASCII_SSS_RENDER_H
#define ASCII_SSS_RENDER_H


#include "../structs.h"
#include "../vector.h"
#include "../sim/simulator.h"


/**
 * Allocates memory for the screenbuffer.
 * @return Successful / error
 */
Error render_init(Screen *screen);
/** Frees the memory of the screenbuffer. */
void render_dispose(Screen *screen);

/** Resets the FPS counters after disabling fullspeed mode. */
void render_resetFPSMeasurement(Screen *screen);

/** Prints the content of the layers on the console. */
void render_refreshScreen(Program *program, Simulation *sim, Screen *screen, LayerStatic *lp);

/** Updates all the layers. */
void render_fullRender(Program *program, Simulation *sim, Screen *screen, LayerStatic *lp, Gui *gui);


#endif //ASCII_SSS_RENDER_H