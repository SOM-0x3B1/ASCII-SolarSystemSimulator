#include "edit_menu.h"
#include "../graphics/drawing.h"
#include "../graphics/layer.h"
#include "../global.h"
#include "../sim/body.h"


#define OPTION_COUNT 10

typedef enum EditMenuOption {
    ADD_BODY = 0,
    EDIT_BODY = 1,
    DELETE_BODY = 2,
    FOLLOW_BODY = 3,
    TOGGLE_DETAILS = 4,
    TOGGLE_G_RANGE = 5,
    TOGGLE_TRAILS = 6,
    IMPORT_SYSTEM = 7,
    EXPORT_SYSTEM = 8,
    EXIT = 9,
} EditMenuOption;


static int cursorPos = 0;
static Layer *ml = &menuLayer;

char *sOptions[OPTION_COUNT] = {"Add body", "Edit body", "Delete body",
                                "Follow body", "Toggle details", "Toggle G range", "Toggle trails",
                                "Import system", "Export system",
                                "Exit"};


void editMenu_switchTo(EconioKey key){
    currentState = EDIT_MENU;
    menuLayer.enabled = true;
    screen_offset.x += 16;

    if(key == KEY_ESCAPE)
        cursorPos = EXIT;
}

void editMenu_renderToggleStatus(int y, bool stat){
    drawing_drawText(ml, screen_width - 13, y, stat ? "[ON] " : "[OFF]");
}

void editMenu_render(){
    drawing_drawLine(ml, screen_width - 32, 2, screen_height - 4, true, '|');
    drawing_drawRectangle(ml, screen_width - 31, 2, screen_width - 1, screen_height-2, ' ');
    drawing_drawText(ml, screen_width - 30, 3, "[EDIT MENU]");

    int yOffset = 0;
    for (int i = 0; i < OPTION_COUNT; ++i) {
        if(i > EXPORT_SYSTEM)
            yOffset = 3;
        else if(i > TOGGLE_TRAILS)
            yOffset = 2;
        else if(i > DELETE_BODY)
            yOffset = 1;

        int y = 5 + i + yOffset;

        drawing_drawText(ml, screen_width - 28, y, sOptions[i]);
        if(i == cursorPos)
            drawing_drawText(ml, screen_width - 30, y, ">");
        else
            drawing_drawText(ml, screen_width - 30, y, " ");

        if(i == TOGGLE_DETAILS)
            editMenu_renderToggleStatus(y, infoLayer.enabled);
        else if(i == TOGGLE_G_RANGE)
            editMenu_renderToggleStatus(y, rangeLayer.enabled);
        else if (i == TOGGLE_TRAILS)
            editMenu_renderToggleStatus(y, trailLayer.enabled);
    }
}


void editMenu_selectOption(){
    switch (cursorPos) {
        case TOGGLE_DETAILS:
            infoLayer.enabled = !infoLayer.enabled;
            break;
        case TOGGLE_G_RANGE:
            rangeLayer.enabled = !rangeLayer.enabled;
            break;
        case TOGGLE_TRAILS:
            trailLayer.enabled = !trailLayer.enabled;
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