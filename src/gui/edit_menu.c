#include "edit_menu.h"
#include "../graphics/drawing.h"
#include "../graphics/layer.h"
#include "../global.h"


#define OPTIONS_COUNT 9

typedef enum EditMenuOption {
    ADD_BODY = 0,
    EDIT_BODY = 1,
    DELETE_BODY = 2,
    FOLLOW_BODY = 3,
    IMPORT_SYSTEM = 4,
    EXPORT_SYSTEM = 5,
    TOGGLE_DETAILS = 6,
    SET_SIM_SPEED = 7,
    EXIT = 8,
} EditMenuOption;


int cursorPos = 0;
Layer *eml = &menuLayer;

char *sOptions[OPTIONS_COUNT] = {"Add body", "Edit body", "Delete body", "Follow body", "Import system",
                     "Export system", "Toggle details", "Set simulation speed", "Exit"};


void editMenu_switchTo(){
    currentState = EDIT_MENU;
    menuLayer.enabled = true;

    screen_offset.x += 16;
}


void editMenu_render(){
    drawing_drawLine(eml, screen_width-32, 2, screen_height-4, true, '|');

    for (int y = 2; y < screen_height-2; ++y)
        drawing_drawLine(eml, screen_width-31, y, 31, false, ' ');

    drawing_drawText(eml, screen_width - 30, 3, "[EDIT MENU]",COL_WHITE);

    for (int i = 0; i < OPTIONS_COUNT; ++i) {
        drawing_drawText(eml, screen_width - 28, 5 + i, sOptions[i],COL_WHITE);
        if(i == cursorPos)
            drawing_drawText(eml, screen_width - 30, 5 + i, ">",COL_WHITE);
        else
            drawing_drawText(eml, screen_width - 30, 5 + i, " ",COL_WHITE);

        if(i == TOGGLE_DETAILS)
            drawing_drawText(eml, screen_width - 13, 5 + i,
                             infoLayer.enabled ? "[ON] " : "[OFF]",COL_WHITE);
    }
}


void editMenu_selectOption(){
    switch (cursorPos) {
        case TOGGLE_DETAILS:
            infoLayer.enabled = !infoLayer.enabled;
            break;
        case EXIT:
            exiting = true;
            break;
    }
}

void editMenu_processInput(){
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        if(key == KEY_ESCAPE || key =='e') {
            currentState = SIMULATION;
            menuLayer.enabled = false;
            cursorPos = 0;
            editMenu_render();

            screen_offset.x -= 16;
        }
        else if ((key == 's' || key == KEY_DOWN) && cursorPos < OPTIONS_COUNT - 1)
            cursorPos++;
        else if ((key == 'w' || key == KEY_UP) && cursorPos > 0)
            cursorPos--;
        else if (key == KEY_ENTER || key == ' ')
            editMenu_selectOption();
    }
}