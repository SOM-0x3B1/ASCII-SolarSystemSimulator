#ifndef ASCII_SSS_EDIT_MENU_H
#define ASCII_SSS_EDIT_MENU_H


#include "../structs.h"
#include "../lib/econio.h"
#include "../sim/body.h"


#define EDIT_MENU_WIDTH 32


/**
 * Set the proper states and display parameters for edit menu.
 * @param key if it's Esc, the cursor jumps to the Exit function
 */
void editMenu_switchTo(EconioKey key, Program *program, Gui *gui, Screen *screen, LayerInstances *li);

/** Renders all edit menu components. */
void editMenu_render(LayerInstances *li, Screen *screen, Gui *gui, Simulation *sim);

/** Reads raw input. */
Error editMenu_processInput(Program *program, Simulation *sim, Screen *screen, Gui *gui, LayerInstances *li);


#endif //ASCII_SSS_EDIT_MENU_H