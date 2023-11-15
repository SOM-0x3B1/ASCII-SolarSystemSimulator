#ifndef ASCII_SSS_BODY_EDITOR_H
#define ASCII_SSS_BODY_EDITOR_H


#include "../structs.h"
#include "../sim/body.h"


/** Set the proper states to read text input for the body editor. */
void bodyEditor_switchTo(Program *p);

/** Renders the current body editor GUI element.*/
void bodyEditor_render(Program *program, LayerInstances *li, Screen *screen, GUI *gui);

/** Processes the input text after pressing enter.*/
void bodyEditor_processTextInput(Program *program, GUI *gui, Simulation *sim);

/** Moves and places the edited body on the simulation window. */
void bodyEditor_processPlacementInput(Program *program, GUI *gui, Simulation *sim);


#endif //ASCII_SSS_BODY_EDITOR_H
