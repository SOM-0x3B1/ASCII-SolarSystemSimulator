#include "edit_menu.h"
#include "../graphics/drawing.h"
#include "../graphics/layer.h"
#include "../global.h"


#define OPTION_COUNT 8

typedef enum EditMenuOption {
    ADD_BODY = 0,
    EDIT_BODY = 1,
    DELETE_BODY = 2,
    FOLLOW_BODY = 3,
    TOGGLE_DETAILS = 4,
    IMPORT_SYSTEM = 5,
    EXPORT_SYSTEM = 6,
    EXIT = 7,
} EditMenuOption;


int cursorPos = 0;
Layer *eml = &menuLayer;

char *sOptions[OPTION_COUNT] = {"Add body", "Edit body", "Delete body", "Follow body", "Toggle details", "Import system",
                                "Export system", "Exit"};


void editMenu_switchTo(EconioKey key){
    currentState = EDIT_MENU;
    menuLayer.enabled = true;
    screen_offset.x += 16;

    if(key == KEY_ESCAPE)
        cursorPos = EXIT;
}


void editMenu_render(){
    drawing_drawLine(eml, screen_width-32, 2, screen_height-4, true, '|');

    for (int y = 2; y < screen_height-2; ++y)
        drawing_drawLine(eml, screen_width-31, y, 31, false, ' ');

    drawing_drawText(eml, screen_width - 30, 3, "[EDIT MENU]");

    int yOffset = 0;
    for (int i = 0; i < OPTION_COUNT; ++i) {
        if(i > EXPORT_SYSTEM)
            yOffset = 3;
        else if(i > TOGGLE_DETAILS)
            yOffset = 2;
        else if(i > DELETE_BODY)
            yOffset = 1;

        drawing_drawText(eml, screen_width - 28, 5 + i + yOffset, sOptions[i]);
        if(i == cursorPos)
            drawing_drawText(eml, screen_width - 30, 5 + i + yOffset, ">");
        else
            drawing_drawText(eml, screen_width - 30, 5 + i + yOffset, " ");

        if(i == TOGGLE_DETAILS)
            drawing_drawText(eml, screen_width - 13, 5 + i + yOffset,
                             infoLayer.enabled ? "[ON] " : "[OFF]");
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
        else if ((key == 's' || key == KEY_DOWN) && cursorPos < OPTION_COUNT - 1)
            cursorPos++;
        else if ((key == 'w' || key == KEY_UP) && cursorPos > 0)
            cursorPos--;
        else if (key == KEY_ENTER || key == ' ')
            editMenu_selectOption();
    }
}