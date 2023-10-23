#include "edit_menu.h"
#include "../graphics/drawing.h"
#include "../graphics/layer.h"
#include "../global.h"
#include "../sim/body.h"


#define MAIN_OPTION_COUNT 10
#define STATE_COUNT 5
#define BODY_PROPERTY_COUNT 6


typedef enum EditMenuMainOption {
    ADD_BODY,
    EDIT_BODY,
    DELETE_BODY,
    FOLLOW_BODY,
    TOGGLE_DETAILS,
    TOGGLE_G_RANGE,
    TOGGLE_TRAILS,
    IMPORT_SYSTEM,
    EXPORT_SYSTEM,
    EXIT,
} EditMenuMainOption;


typedef enum EditMenuSTATE {
    STATE_MAIN,
    STATE_ADD_BODY,
    STATE_EDIT_BODY,
    STATE_DELETE_BODY,
    STATE_FOLLOW_BODY,
} EditMenuSTATE;


static EditMenuSTATE state = STATE_MAIN;
static int cursorPos = 0;

static Layer *ml = &menuLayer;

char *sMainOptions[MAIN_OPTION_COUNT] = {"Add body", "Edit body", "Delete body",
                                         "Follow body", "Toggle details", "Toggle G range", "Toggle trails",
                                         "Import system", "Export system",
                                         "Exit"};

int STATEOptionCounts[STATE_COUNT] = {MAIN_OPTION_COUNT, BODY_PROPERTY_COUNT, BODY_PROPERTY_COUNT,
                                        -1, -1,}; // 1: count of bodies


void editMenu_switchTo(EconioKey key){
    programState = EDIT_MENU;
    state = STATE_MAIN;

    menuLayer.enabled = true;
    screen_offset.x += 16;

    if(key == KEY_ESCAPE)
        cursorPos = EXIT;
}

void editMenu_renderToggleSTATE(int y, bool stat){
    drawing_drawText(ml, screen_width - 13, y, stat ? "[ON] " : "[OFF]");
}

void editMenu_renderSelection(int i, int y){
    if(i == cursorPos)
        drawing_drawText(ml, screen_width - 30, y, ">");
    else
        drawing_drawText(ml, screen_width - 30, y, " ");
}

void editMenu_renderMain(){
    int yOffset = 0;
    for (int i = 0; i < MAIN_OPTION_COUNT; ++i) {
        if(i > EXPORT_SYSTEM)
            yOffset = 3;
        else if(i > TOGGLE_TRAILS)
            yOffset = 2;
        else if(i > DELETE_BODY)
            yOffset = 1;

        int y = 5 + i + yOffset;

        drawing_drawText(ml, screen_width - 28, y, sMainOptions[i]);
        editMenu_renderSelection(i, y);

        if(i == TOGGLE_DETAILS)
            editMenu_renderToggleSTATE(y, infoLayer.enabled);
        else if(i == TOGGLE_G_RANGE)
            editMenu_renderToggleSTATE(y, rangeLayer.enabled);
        else if (i == TOGGLE_TRAILS)
            editMenu_renderToggleSTATE(y, trailLayer.enabled);
    }
}

void editMenu_renderBodyList(){
    for (int i = 0; i < bodyArray.length; ++i) {
        int y = 5 + i;

        drawing_drawText(ml, screen_width - 28, y, bodyArray.data[i].name);
        editMenu_renderSelection(i, y);
    }

    drawing_drawText(ml, screen_width - 28, 5 + bodyArray.length + 1, "Back");
    editMenu_renderSelection(bodyArray.length, 5 + bodyArray.length + 1);
}

void editMenu_render(){
    drawing_drawLine(ml, screen_width - 32, 2, screen_height - 4, true, '|');
    drawing_drawRectangle(ml, screen_width - 31, 2, screen_width - 1, screen_height-2, ' ');
    drawing_drawText(ml, screen_width - 30, 3, "[EDIT MENU]");

    switch (state) {
        case STATE_MAIN:
            editMenu_renderMain();
            break;
        case STATE_FOLLOW_BODY:
            editMenu_renderBodyList();
            break;
    }
}

void editMenu_selectMainOption(){
    switch (cursorPos) {
        case FOLLOW_BODY:
            cursorPos = 0;
            state = STATE_FOLLOW_BODY;
            break;
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

void editMenu_selectFollowOption(){
    if(cursorPos == bodyArray.length){
        state = STATE_MAIN;
        cursorPos = 0;
    } else
        following = &bodyArray.data[cursorPos];
}

void editMenu_processInput(){
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        int cursorMax = STATEOptionCounts[state];
        if(cursorMax < 0)
            cursorMax = bodyArray.length + 1;

        if(key == KEY_ESCAPE || key =='e') {
            programState = SIMULATION;
            state = STATE_MAIN;
            menuLayer.enabled = false;
            cursorPos = 0;
            editMenu_render();

            screen_offset.x -= 16;
        }
        else if ((key == 's' || key == KEY_DOWN) && cursorPos < cursorMax - 1)
            cursorPos++;
        else if ((key == 'w' || key == KEY_UP) && cursorPos > 0)
            cursorPos--;
        else if (key == KEY_ENTER || key == ' ') {
            switch (state) {
                case STATE_MAIN:
                    editMenu_selectMainOption();
                    break;
                case STATE_FOLLOW_BODY:
                    editMenu_selectFollowOption();
                    break;
            }
        }
    }
}