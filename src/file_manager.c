#include <stdio.h>
#include <string.h>
#include "file_manager.h"
#include "global.h"
#include "sim/body.h"


int getINIParam(char* dest, const char* src) {
    int i;
    for (i = 0; src[i] != '='; ++i)
        dest[i] = src[i];
    dest[i] = '\0';

    return i;
}

int settings_getIntValue(const char* src, int start, int *value) {
    *value = 0;
    int i;
    for (i = start + 1; src[i] != '\0'; ++i) {
        if(src[i] < '0' || src[i] > '9')
            return 1;
        *value = *value * 10 + (src[i] - '0');
    }
    return 0;
}

int settings_loadSettings() {
    FILE *f;
    f = fopen("settings.ini", "r");

    if(f != NULL) {
        char line[32];
        while (fscanf(f, "%s", line) != EOF) {
            char param[16];
            int valueStart = getINIParam(param, line);

            int value;
            if (settings_getIntValue(line, valueStart, &value) != 0)
                return 3; // invalid value

            if (strcmp(param, "screen_width") == 0)
                screen_width = value;
            else if (strcmp(param, "screen_height") == 0)
                screen_height = value;
            else if (strcmp(param, "targetFPS") == 0) {
                if(value > 0)
                    targetFPS = value;
                else
                    return 4; // target fps must be higher than 0
            }
            else if (strcmp(param, "solarMass") == 0) {
                if(value > 0)
                    solarMass = value;
                else
                    return 5; // sunmass must be higher than 0
            }
            else
                return 2; // invalid parameter
        }
        fclose(f);
    } else
        return 1; // unable to open

    return 0;
}