#include "error.h"
#include "string.h"
#include "../lib/econio.h"
#include "../graphics/drawing.h"


/** Converts an error code into an informative string message. */
static void errToString(char *res, Error error) {
    switch (error) {
        case ERR_MEMORY:
            strcpy(res, "CRITICAL ERROR: Unable to allccate memory.");
            break;
        case ERR_SETTINGS_OPEN_FILE:
            strcpy(res, "Unable to open settings.ini.");
            break;
        case ERR_SETTINGS_PARAMETER:
            strcpy(res, "Invalid parameter name in settings.");
            break;
        case ERR_SETTIGNS_VALUE:
            strcpy(res, "Invalid value in settings.");
            break;
        case ERR_FS_FILENAME:
            strcpy(res, "Invalid file name.");
            break;
        case ERR_ADDBODY_VALUE:
            strcpy(res, "Invalid body parameter.");
            break;
        case ERR_EXPORT_WRITE:
            strcpy(res, "Unable to create export file.");
            break;
        case ERR_IMPORT_OPEN_FILE:
            strcpy(res, "Unable to open import file.");
            break;
        case ERR_IMPORT_VALUE:
            strcpy(res, "Invalid body parameter in import file.");
            break;
        case ERR_MAIN_MENU_ART_MISSING:
            strcpy(res, "Missing main menu art files.");
            break;
        default:
            strcpy(res, "Unkown error.");
            break;
    }
}

void error_render(Error error, Screen *screen, LayerInstances *li) {
    char errString[45];
    errToString(errString, error);
    int msgLength = (int) strlen(errString);

    if (msgLength < 26)
        msgLength = 26;

    int x1 = screen->width / 2 - msgLength / 2 - 3;
    int yCenter = screen->height / 2 + 1;

    drawing_drawRectangleOutline(&li->overlayLayer, x1 - 1, yCenter - 5,
                                 x1 + msgLength + 5, yCenter + 2, screen);
    int xCentrer = drawing_drawBox(&li->overlayLayer, x1, yCenter - 6,
                                   x1 + msgLength + 4, yCenter + 1, "ERROR", screen);
    drawing_drawText(&li->overlayLayer, x1 + 2, yCenter - 2, errString, screen);
    drawing_drawText(&li->overlayLayer,
                     xCentrer - (error != ERR_MEMORY ? 13 : 10), yCenter,
                     error != ERR_MEMORY ? "Press 'ENTER' to continue" : "Press 'ENTER' to abort", screen);
}


void error_awaitConfirmation() {
    int key = 0;
    econio_rawmode();
    while (key != KEY_ENTER) {
        if (econio_kbhit()) {
            while (econio_kbhit())
                key = econio_getch();
            econio_sleep(0.02);
        }
    }
}