#include <stdio.h>
#include <string.h>
#include "fs.h"
#include "sim/body.h"
#include "graphics/drawing.h"


#define MAX_FILENAME_LENGTH 250

/** Copies the string name of the parameter into the destination from the source. */
static void getParam(char *dest, const char *src){
    int i = 0, j = 0;
    for (; src[i] != '\0' && src[i] != '=' ; ++i)
        dest[j++] = src[i];
    dest[j] = '\0';
}

/** Copies the value string into the destination from the source. */
static void getSValue(char *dest, const char *src){
    int i = 0, j = 0;
    while (src[i] != '\0' && src[i] != '=')
        i++;
    i++;
    for (; src[i] != '\0'; ++i)
        dest[j++] = src[i];
    dest[j] = '\0';
}

Error settings_loadSettings(Simulation *sim, Screen *screen) {
    FILE *f;
    f = fopen("settings.ini", "r");

    if(f != NULL) {
        char line[32];
        while (fscanf(f, "%s", line) != EOF) {
            char param[32];
            char sValue[32];

            getParam(param, line);
            getSValue(sValue, line);

            double value = 0;
            if (sscanf(sValue, "%lf", &value) != 1)
                return ERR_SETTIGNS_VALUE; // invalid value

            if (strcmp(param, "screen_width") == 0) {
                if (value > 0) screen->width = (int) value;
                else return ERR_SETTIGNS_VALUE;
            } else if (strcmp(param, "screen_height") == 0) {
                if (value > 0) screen->height = (int) value;
                else return ERR_SETTIGNS_VALUE;
            } else if (strcmp(param, "targetFPS") == 0) {
                if (value > 0) screen->targetFPS = (int) value;
                else return ERR_SETTIGNS_VALUE;
            } else if (strcmp(param, "solarMass") == 0) {
                if (value > 0) sim->solarMass = value;
                else return ERR_SETTIGNS_VALUE;
            } else if (strcmp(param, "detectCollisionPercentage") == 0) {
                if (value > 0) sim->detectCollisionPercentage = (double) value / 100;
                else return ERR_SETTIGNS_VALUE;
            } else
                return ERR_SETTINGS_PARAMETER; // invalid parameter
        }
        fclose(f);
    } else
        return ERR_SETTINGS_OPEN_FILE; // unable to open

    return SUCCESS;
}


void export_switchTo(Program *program){
    program->state = PROGRAM_STATE_TEXT_INPUT;
    program->textInputDest = TEXT_INPUT_EXPORT;
}


void export_render(Gui *gui, LayerInstances *li, Screen *screen){
    gui->textPos = drawing_drawInputPrompt(&li->menuLayer, screen->height / 2 - 2, "Export system", "Name:", screen);
}


/** Scans the input file name for illegal characters. */
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


/**
 * Creates a new file, and writes the content of the body array into it.
 * @return Success / error
 */
static Error export_export(char *filename, Simulation *sim) {
    FILE *f;
    f = fopen(strcat(filename, ".tsv"), "w");
    if (f != NULL) {
        fprintf(f, "solar-mass\n%lf\n\n", sim->solarMass);

        fprintf(f, "name\tmass\tpos-x\tpos-y\tradius\tvel-x\tvel-y\tcolor");
        for (int i = 0; i < sim->bodyArray.length; ++i) {
            Body *b = &sim->bodyArray.data[i];
            fprintf(f, "\n%s\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%c",
                    b->name, b->mass, b->position.x, b->position.y,
                    b->r, b->velocity.x, b->velocity.y, b->color);
        }
        fclose(f);
    } else
        return ERR_EXPORT_WRITE; // unable to create file

    return SUCCESS;
}


Error export_processTextInput(Gui *gui, Program *program, Simulation *sim) {
    econio_gotoxy((int) gui->textPos.x, (int) gui->textPos.y);
    econio_normalmode();

    char filename[MAX_FILENAME_LENGTH + 5];
    scanf("%s", filename);

    econio_rawmode();
    econio_gotoxy(0, 0);

    program->state = PROGRAM_STATE_EDIT_MENU;

    if(checkFilename(filename) == 0)
        return export_export(filename, sim);
    else
        return ERR_FS_FILENAME;
}