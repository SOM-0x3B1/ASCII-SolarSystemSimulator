#ifndef ASCII_SSS_FS_H
#define ASCII_SSS_FS_H


#include "structs.h"
#include "sim/simulator.h"
#include "graphics/render.h"
#include "graphics/layer.h"


/** Loads the settings from setting.ini.
 *  @return Successful / error
 */
Error fs_settings_loadSettings(Simulation *sim, Screen *screen);

/** Loads the earth animation.
 *  @return Successful / error
 */
Error fs_loadMainMenu(Simulation *sim, Screen *screen, Gui *gui);

/** Sets the proper program states to read input text for export. */
void fs_export_switchTo(Program *program);

/** Renders the input prompt. */
void fs_export_render(Gui *gui, LayerInstances *li, Screen *screen);

/** Reads the name of the export file. */
Error fs_export_processTextInput(Gui *gui, Program *program, Simulation *sim);


#endif //ASCII_SSS_FS_H
