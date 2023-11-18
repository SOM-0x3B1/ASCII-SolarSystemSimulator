#ifndef ASCII_SSS_FS_H
#define ASCII_SSS_FS_H


#include "structs.h"
#include "sim/simulator.h"
#include "graphics/render.h"
#include "graphics/layer.h"


/** Loads the settings from setting.ini.
 *  @return Successful / error
 */
Error settings_loadSettings(Simulation *sim, Screen *screen);

/** Sets the proper program states to read input text for export. */
void export_switchTo(Program *program);

/** Renders the input prompt. */
void export_render(Gui *gui, LayerInstances *li, Screen *screen);

/** Reads the name of the export file. */
Error export_processTextInput(Gui *gui, Program *program, Simulation *sim);


#endif //ASCII_SSS_FS_H
