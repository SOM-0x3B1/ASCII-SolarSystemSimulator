#ifndef ASCII_SSS_FS_H
#define ASCII_SSS_FS_H


int getINIParam(char* dest, const char* src);

int settings_getIntValue(const char* src, int start, int *value);

int settings_loadSettings();

void export_setState();

void export_render();

void export_processTextInput();


#endif //ASCII_SSS_FS_H
