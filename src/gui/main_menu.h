#ifndef ASCII_SSS_MAIN_MENU_H
#define ASCII_SSS_MAIN_MENU_H


#include "../structs.h"
#include "../lib/econio.h"
#include "../graphics/drawing.h"

void mainMenu_render(Screen *screen, LayerInstances *li, Gui *gui);

void mainMenu_processInput(Program *program, LayerInstances *li);


#endif //ASCII_SSS_MAIN_MENU_H
