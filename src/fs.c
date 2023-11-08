#include <stdio.h>
#include <string.h>
#include "fs.h"
#include "global.h"
#include "sim/body.h"
#include "graphics/drawing.h"


#define MAX_FILENAME_LENGTH 250


static Point textPos;


static int getINIParam(char* dest, const char* src) {
    int i;
    for (i = 0; src[i] != '='; ++i)
        dest[i] = src[i];
    dest[i] = '\0';

    return i;
}

static int settings_getIntValue(const char* src, int start, int *value) {
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
            char param[32];
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
            } else if (strcmp(param, "solarMass") == 0) {
                if(value > 0)
                    solarMass = value;
                else
                    return 5; // sunmass must be higher than 0
            } else if (strcmp(param, "detectCollisionPercentage") == 0) {
                if(value > 0)
                    detectCollisionPercentage = (double)value / 100;
                else
                    return 6; // sunmass must be higher than 0
            } else
                return 2; // invalid parameter
        }
        fclose(f);
    } else
        return 1; // unable to open

    return 0;
}


void export_setState(){
    programState = TEXT_INPUT;
    textInputDest = TEXT_INPUT_EXPORT;
}

void export_render(){
    textPos = drawing_drawInputPrompt(&menuLayer, screen_height / 2 - 2, "Export system", "Name:");
}


static int checkFilename(const char *fn){
    int i = 0;
    for (; fn[i] != '\0' && i < MAX_FILENAME_LENGTH; ++i) {
        char c = fn[i];
        if(c != '-' && c != '_' && c !='.' && (c < '0' || (c > '9' && c < 'A') || (c > 'Z' && c < 'a') || (c > 'z')))
            return 1; // invalid character
    }
    if(fn[i] != '\0')
        return 2; // invalid length

    return 0;
}

int export_export(char *filename) {
    FILE *f;
    f = fopen(strcat(filename, ".tsv"), "w");
    if (f != NULL) {
        fprintf(f, "solar-mass\n%lf\n\n", solarMass);

        fprintf(f, "name\tmass\tpos-x\tpos-y\tradius\tvel-x\tvel-y\tcolor");
        for (int i = 0; i < bodyArray.length; ++i) {
            Body *b = &bodyArray.data[i];
            fprintf(f, "\n%s\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%c",
                    b->name, b->mass, b->position.x, b->position.y,
                    b->r, b->velocity.x, b->velocity.y, b->color);
        }
        fclose(f);
    } else
        return 1; // unable to create file

    return 0;
}

void export_processTextInput() {
    econio_gotoxy((int) textPos.x, (int) textPos.y);
    econio_normalmode();

    char filename[MAX_FILENAME_LENGTH + 5];
    scanf("%s", filename);

    if(checkFilename(filename) == 0)
        export_export(filename);

    econio_rawmode();
    econio_gotoxy(0, 0);

    programState = EDIT_MENU;
}