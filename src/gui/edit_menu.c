#include "edit_menu.h"
#include "../graphics/drawing.h"
#include "../global.h"
#include "../sim/body.h"
#include "body_editor.h"
#include "stdio.h"
#include "../fs.h"


#define EDIT_MENU_MAIN_OPTION_COUNT 10
#define EDIT_MENU_STATE_COUNT 6
#define BODY_PROPERTY_COUNT 5


typedef enum EditMenuMainOption {
    OPTION_ADD_BODY,
    OPTION_EDIT_BODY,
    OPTION_DELETE_BODY,
    OPTION_FOLLOW_BODY,
    OPTION_TOGGLE_DETAILS,
    OPTION_TOGGLE_G_RANGE,
    OPTION_TOGGLE_TRAILS,
    OPTION_IMPORT_SYSTEM,
    OPTION_EXPORT_SYSTEM,
    OPTION_EXIT,
} EditMenuMainOption;


EditMenuSTATE editMenu_state = STATE_MAIN;


static int cursorPos = 0; // The current position of the selection cursor


// How many options should the given state render.
static int stateOptionCounts[EDIT_MENU_STATE_COUNT] = {EDIT_MENU_MAIN_OPTION_COUNT, BODY_PROPERTY_COUNT + 1, -1, BODY_PROPERTY_COUNT + 1,
                                                       -1, -1,}; // -1: count of bodies

// The option strings stored for each main option
static char *sMainOptions[EDIT_MENU_MAIN_OPTION_COUNT] = {"Add body", "Edit body", "Delete body", "Follow body",
                                                          "Toggle details", "Toggle G range", "Toggle trails",
                                                          "Import system", "Export system", "Exit"};

// The body property strings stored for each body property
static char *sBodySettingOptions[BODY_PROPERTY_COUNT] = {"Set name", "Set mass", "Set size",
                                                  "Set position", "Set velocity"};


void editMenu_switchTo(EconioKey key){
    programState = EDIT_MENU;
    editMenu_state = STATE_MAIN;

    menuLayer.enabled = true;
    screen_offset.x += EDIT_MENU_WIDTH / 2;

    if(key == KEY_ESCAPE)
        cursorPos = OPTION_EXIT;
}


/** Renders the "[ON]"/"[OFF]" texts next to toggle buttons. */
static void editMenu_renderToggleOptionSTATE(int y, bool stat){
    drawing_drawText(&menuLayer, screen_width - 13, y, stat ? "[ON] " : "[OFF]");
}


/** Renders the '>' sign next to the selected option. */
static void editMenu_renderSelection(int i, int y){
    if(i == cursorPos)
        drawing_drawText(&menuLayer, screen_width - 30, y, ">");
    else
        drawing_drawText(&menuLayer, screen_width - 30, y, " ");
}


/** Renders the list of main options. */
static void editMenu_renderMain(){
    int yOffset = 0;
    for (int i = 0; i < EDIT_MENU_MAIN_OPTION_COUNT; ++i) {
        if(i > OPTION_EXPORT_SYSTEM)
            yOffset = 3;
        else if(i > OPTION_TOGGLE_TRAILS)
            yOffset = 2;
        else if(i > OPTION_DELETE_BODY)
            yOffset = 1;

        int y = 5 + i + yOffset;

        drawing_drawText(&menuLayer, screen_width - 28, y, sMainOptions[i]);
        editMenu_renderSelection(i, y);

        if(i == OPTION_TOGGLE_DETAILS)
            editMenu_renderToggleOptionSTATE(y, infoLayer.enabled);
        else if(i == OPTION_TOGGLE_G_RANGE)
            editMenu_renderToggleOptionSTATE(y, rangeLayer.enabled);
        else if (i == OPTION_TOGGLE_TRAILS)
            editMenu_renderToggleOptionSTATE(y, trailLayer.enabled);
    }
}


/** Renders the list of bodies. */
static void editMenu_renderBodyList(){
    for (int i = 0; i < bodyArray.length; ++i) {
        int y = 5 + i;

        drawing_drawText(&menuLayer, screen_width - 28, y, bodyArray.data[i].name);
        editMenu_renderSelection(i, y);
    }

    drawing_drawText(&menuLayer, screen_width - 28, 5 + bodyArray.length + 1, "Back");
    editMenu_renderSelection(bodyArray.length, 5 + bodyArray.length + 1);
}


/** Renders the list of edit body properties. */
static void editMenu_renderEditProperties(){
    drawing_drawText(&menuLayer, screen_width - 30, 5, editedBody->name);

    for (int i = 0; i < BODY_PROPERTY_COUNT; ++i) {
        int y = 6 + i;

        drawing_drawText(&menuLayer, screen_width - 28, y, sBodySettingOptions[i]);
        editMenu_renderSelection(i, y);
    }

    drawing_drawText(&menuLayer, screen_width - 28, 6 + BODY_PROPERTY_COUNT + 1, "Accept");
    editMenu_renderSelection(BODY_PROPERTY_COUNT, 6 + BODY_PROPERTY_COUNT + 1);
}


void editMenu_render(){
    layer_clear(&menuLayer);

    drawing_drawLine(&menuLayer, screen_width - 32, 2, screen_height - 4, true, '|');
    drawing_drawRectangle(&menuLayer, screen_width - 31, 2, screen_width - 1, screen_height-2, ' ');
    drawing_drawText(&menuLayer, screen_width - 30, 3, "[EDIT MENU]");

    switch (editMenu_state) {
        case STATE_MAIN:
            editMenu_renderMain();
            break;
        case STATE_ADD_BODY:
        case STATE_EDIT_BODY:
        case STATE_DELETE_BODY:
        case STATE_FOLLOW_BODY:
            editMenu_renderBodyList();
            break;
        case STATE_EDIT_BODY_SET:
            editMenu_renderEditProperties();
            break;
        default:
            break;
    }
}


/** Enter on main menu option */
static void editMenu_selectMainOption(){
    EditMenuSTATE lastState = editMenu_state;

    switch (cursorPos) {
        case OPTION_ADD_BODY:
            editMenu_state = STATE_ADD_BODY;
            bodyEditor_state = BODY_SET_NAME;
            bodyEditor_switchTo();
            editedBody = body_new("", (Vector) {
                                          (double) screen_offset.x + (double) (screen_width) / 2 - (double)EDIT_MENU_WIDTH / 2,
                                          ((double) screen_offset.y + (double) screen_height / 2) * 2},
                                  (Vector) {0, 0}, 0.0, 0.0, '#');
            following = editedBody;
            break;
        case OPTION_DELETE_BODY:
            editMenu_state = STATE_DELETE_BODY;
            break;
        case OPTION_EDIT_BODY:
            editMenu_state = STATE_EDIT_BODY;
            break;
        case OPTION_FOLLOW_BODY:
            editMenu_state = STATE_FOLLOW_BODY;
            break;
        case OPTION_TOGGLE_DETAILS:
            infoLayer.enabled = !infoLayer.enabled;
            break;
        case OPTION_TOGGLE_G_RANGE:
            rangeLayer.enabled = !rangeLayer.enabled;
            break;
        case OPTION_TOGGLE_TRAILS:
            trailLayer.enabled = !trailLayer.enabled;
            break;
        case OPTION_EXPORT_SYSTEM:
            export_switchTo();
            break;
        case OPTION_EXIT:
            exiting = true;
            break;
        default:
            break;
    }

    if(lastState != editMenu_state)
        cursorPos = 0;
}


/** Enter on "Edit body"/[body] */
static void editMenu_selectEditOption(){
    if(cursorPos == bodyArray.length){
        editMenu_state = STATE_MAIN;
        cursorPos = 0;
    } else{
        editedBody = &bodyArray.data[cursorPos];
        following = editedBody;
        editMenu_state = STATE_EDIT_BODY_SET;
        cursorPos = 0;
    }
}


/** Enter on "Edit body"/[body]/[property] */
static void editMenu_selectEditPropertyOption(){
    if(cursorPos == BODY_PROPERTY_COUNT){
        cursorPos = 0;
        editMenu_state = STATE_EDIT_BODY;
    } else {
        bodyEditor_state = cursorPos;
        if(cursorPos != BODY_SET_POS)
            bodyEditor_switchTo();
        else
            programState = PLACING_BODY;
    }
}


/** Enter on "Delete body"/[body] */
static void editMenu_selectDeleteOption(){
    if(cursorPos == bodyArray.length){
        editMenu_state = STATE_MAIN;
        cursorPos = 0;
    } else {
        if(following == &bodyArray.data[cursorPos])
            following = NULL;
        bodyArray_removeAt(cursorPos);
        if(cursorPos == bodyArray.length && cursorPos > 0)
            cursorPos--;
    }
}


/** Enter on "Follow body"/[body] */
static void editMenu_selectFollowOption(){
    if(cursorPos == bodyArray.length){
        editMenu_state = STATE_MAIN;
        cursorPos = 0;
    } else
        following = &bodyArray.data[cursorPos];
}


/** Closes the edit menu and returns to simulation mode. */
static void editMenu_close(){
    programState = SIMULATION;
    editMenu_state = STATE_MAIN;
    menuLayer.enabled = false;
    cursorPos = 0;
    editMenu_render();

    screen_offset.x -= EDIT_MENU_WIDTH / 2;
}


void editMenu_processInput(){
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        int cursorMax = stateOptionCounts[editMenu_state];
        if(cursorMax < 0)
            cursorMax = bodyArray.length + 1;

        if(key == KEY_ESCAPE || key =='e')
            editMenu_close();
        else if ((key == 's' || key == KEY_DOWN) && cursorPos < cursorMax - 1)
            cursorPos++;
        else if ((key == 'w' || key == KEY_UP) && cursorPos > 0)
            cursorPos--;
        else if (key == KEY_ENTER || key == ' ') {
            switch (editMenu_state) {
                case STATE_MAIN:
                    editMenu_selectMainOption();
                    break;
                case STATE_EDIT_BODY:
                    editMenu_selectEditOption();
                    break;
                case STATE_EDIT_BODY_SET:
                    editMenu_selectEditPropertyOption();
                    break;
                case STATE_DELETE_BODY:
                    editMenu_selectDeleteOption();
                    break;
                case STATE_FOLLOW_BODY:
                    editMenu_selectFollowOption();
                    break;
                default:
                    break;
            }
        }
    }
}