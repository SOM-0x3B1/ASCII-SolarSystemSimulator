#ifndef ASCII_SSS_FILE_MANAGER_H
#define ASCII_SSS_FILE_MANAGER_H


int getINIParam(char* dest, const char* src);

int settings_getIntValue(const char* src, int start, int *value);

int settings_loadSettings();


#endif //ASCII_SSS_FILE_MANAGER_H
