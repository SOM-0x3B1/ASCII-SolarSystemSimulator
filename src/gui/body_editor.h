#ifndef ASCII_SSS_BODY_EDITOR_H
#define ASCII_SSS_BODY_EDITOR_H


#include "../structs.h"
#include "../sim/body.h"


/** Set the proper states to read text input for the body editor. */
void bodyEditor_switchTo(Program *p);

/** Renders the current body editor Gui element.*/
void bodyEditor_render(Program *program, LayerInstances *li, Screen *screen, Gui *gui);

/** Processes the input text after pressing enter.
 *  @return Success / error
 */
Error bodyEditor_processTextInput(Program *program, Gui *gui, Simulation *sim, Screen *screen);

/** Moves and places the edited body on the simulation window. */
void bodyEditor_processPlacementInput(Program *program, Gui *gui, Simulation *sim);


#endif //ASCII_SSS_BODY_EDITOR_H
