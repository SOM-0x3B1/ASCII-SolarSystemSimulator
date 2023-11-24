#ifndef ASCII_SSS_MAIN_MENU_H
#define ASCII_SSS_MAIN_MENU_H


#include "../structs.h"
#include "../lib/econio.h"
#include "../graphics/drawing.h"


/** Renders the main menu. */
void mainMenu_render(Screen *screen, LayerInstances *li, Gui *gui);

/** Closes the main menu and starts the simulation */
void mainMenu_startSim(Program *program, LayerInstances *li);

/** Calls mainMenu_startSim() if ENTER is pressed. */
void mainMenu_processInput(Program *program, LayerInstances *li);


#endif //ASCII_SSS_MAIN_MENU_H
