#ifndef ASCII_SSS_FS_H
#define ASCII_SSS_FS_H


/** Loads the settings from setting.ini. */
int settings_loadSettings();

/** Sets the proper program states to read input text for export. */
void export_switchTo();

/** Renders the input prompt. */
void export_render();

/** Reads the name of the export file. */
void export_processTextInput();


#endif //ASCII_SSS_FS_H
