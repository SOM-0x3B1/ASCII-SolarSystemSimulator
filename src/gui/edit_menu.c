#include "edit_menu.h"
#include "../graphics/drawing.h"
#include "body_editor.h"
#include "stdio.h"
#include "../fs.h"


/*#define EDIT_MENU_MAIN_OPTION_COUNT 10
#define EDIT_MENU_STATE_COUNT 6*/


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
    EditMenuMainOption_MAX
} EditMenuMainOption;


void editMenu_switchTo(EconioKey key, Program *program, GUI *gui, Screen *screen, LayerInstances *li){
    program->programState = EDIT_MENU;
    gui->editMenu_state = EDIT_MENU_STATE_MAIN;

    li->menuLayer.enabled = true;
    screen->screen_offset.x += EDIT_MENU_WIDTH / 2;

    if(key == KEY_ESCAPE)
        gui->cursorPos = OPTION_EXIT;
}


/** Renders the "[ON]"/"[OFF]" texts next to toggle buttons. */
static void editMenu_renderToggleOptionSTATE(int y, bool stat, LayerInstances *li, Screen *screen){
    drawing_drawText(&li->menuLayer, screen->screen_width - 13, y, stat ? "[ON] " : "[OFF]", screen);
}


/** Renders the '>' sign next to the selected option. */
static void editMenu_renderSelection(int i, int y, GUI *gui, LayerInstances *li, Screen *screen){
    if(i == gui->cursorPos)
        drawing_drawText(&li->menuLayer, screen->screen_width - 30, y, ">", screen);
    else
        drawing_drawText(&li->menuLayer, screen->screen_width - 30, y, " ", screen);
}


static char* mainOptionToString(EditMenuMainOption o){
    switch (o) {
        case OPTION_ADD_BODY:
            return "Add body";
        case OPTION_EDIT_BODY:
            return "Edit body";
        case OPTION_DELETE_BODY:
            return "Delete body";
        case OPTION_FOLLOW_BODY:
            return "Follow body";
        case OPTION_TOGGLE_DETAILS:
            return "Toggle details";
        case OPTION_TOGGLE_G_RANGE:
            return "Toggle G range";
        case OPTION_TOGGLE_TRAILS:
            return "Toggle trails";
        case OPTION_IMPORT_SYSTEM:
            return "Import system";
        case OPTION_EXPORT_SYSTEM:
            return "Export system";
        default:
            return "Exit";
    }
}


/** Renders the list of main options. */
static void editMenu_renderMain(LayerInstances *li, GUI *gui, Screen *screen){
    int yOffset = 0;
    for (int i = 0; i < EditMenuMainOption_MAX; ++i) {
        if(i > OPTION_EXPORT_SYSTEM)
            yOffset = 3;
        else if(i > OPTION_TOGGLE_TRAILS)
            yOffset = 2;
        else if(i > OPTION_DELETE_BODY)
            yOffset = 1;

        int y = 5 + i + yOffset;

        drawing_drawText(&li->menuLayer, screen->screen_width - 28, y, mainOptionToString(i), screen);
        editMenu_renderSelection(i, y, gui, li, screen);

        if(i == OPTION_TOGGLE_DETAILS)
            editMenu_renderToggleOptionSTATE(y, li->infoLayer.enabled, li, screen);
        else if(i == OPTION_TOGGLE_G_RANGE)
            editMenu_renderToggleOptionSTATE(y, li->rangeLayer.enabled, li, screen);
        else if (i == OPTION_TOGGLE_TRAILS)
            editMenu_renderToggleOptionSTATE(y, li->trailLayer.enabled, li, screen);
    }
}


/** Renders the list of bodies. */
static void editMenu_renderBodyList(Simulation *sim, LayerInstances *li, Screen *screen, GUI *gui){
    for (int i = 0; i < sim->bodyArray.length; ++i) {
        int y = 5 + i;

        drawing_drawText(&li->menuLayer, screen->screen_width - 28, y, sim->bodyArray.data[i].name, screen);
        editMenu_renderSelection(i, y, gui, li, screen);
    }

    drawing_drawText(&li->menuLayer, screen->screen_width - 28, 5 + sim->bodyArray.length + 1, "Back", screen);
    editMenu_renderSelection(sim->bodyArray.length, 5 + sim->bodyArray.length + 1, gui, li, screen);
}


static char* bodyOptionToString(BodyEditableProperty p){
    switch (p) {
        case BODY_PROPERTY_NAME:
            return "Set name";
        case BODY_PROPERTY_MASS:
            return "Set mass";
        case BODY_PROPERTY_R:
            return "Set radius";
        case BODY_PROPERTY_POS:
            return "Set position";
        default:
            return "Set velocity";
    }
}


/** Renders the list of edit body properties. */
static void editMenu_renderEditProperties(LayerInstances *li, Screen *screen, Simulation *sim, GUI *gui){
    drawing_drawText(&li->menuLayer, screen->screen_width - 30, 5, sim->editedBody->name, screen);

    for (int i = 0; i < bodyEditableProperty_MAX; ++i) {
        int y = 6 + i;

        drawing_drawText(&li->menuLayer, screen->screen_width - 28, y, bodyOptionToString(i), screen);
        editMenu_renderSelection(i, y, gui, li, screen);
    }

    drawing_drawText(&li->menuLayer, screen->screen_width - 28, 6 + bodyEditableProperty_MAX + 1, "Accept", screen);
    editMenu_renderSelection(bodyEditableProperty_MAX, 6 + bodyEditableProperty_MAX + 1, gui, li, screen);
}


void editMenu_render(LayerInstances *li, Screen *screen, GUI *gui, Simulation *sim){
    layer_clear(&li->menuLayer, screen);

    drawing_drawLine(&li->menuLayer, screen->screen_width - 32, 2,
                     screen->screen_height - 4, true, '|', screen);
    drawing_drawRectangle(&li->menuLayer, screen->screen_width - 31, 2,
                          screen->screen_width - 1, screen->screen_height-2, ' ', screen);
    drawing_drawText(&li->menuLayer, screen->screen_width - 30, 3, "[EDIT MENU]", screen);

    switch (gui->editMenu_state) {
        case EDIT_MENU_STATE_MAIN:
            editMenu_renderMain(li, gui, screen);
            break;
        case EDIT_MENU_STATE_ADD_BODY:
        case EDIT_MENU_STATE_EDIT_BODY:
        case EDIT_MENU_STATE_DELETE_BODY:
        case EDIT_MENU_STATE_FOLLOW_BODY:
            editMenu_renderBodyList(sim, li, screen, gui);
            break;
        case EDIT_MENU_STATE_EDIT_BODY_SET:
            editMenu_renderEditProperties(li, screen, sim, gui);
            break;
        default:
            break;
    }
}


/** Enter on main menu option */
static void editMenu_selectMainOption(GUI *gui, Simulation *sim, Screen *screen, LayerInstances *li, Program *program){
    EditMenuSTATE lastState = gui->editMenu_state;

    switch (gui->cursorPos) {
        case OPTION_ADD_BODY:
            gui->editMenu_state = EDIT_MENU_STATE_ADD_BODY;
            gui->bodyEditor_state = BODY_SET_NAME;
            bodyEditor_switchTo(program);
            sim->editedBody =
                    body_new("",
                             (Vector) {(double) screen->screen_offset.x + (double) screen->screen_width / 2 - (double)EDIT_MENU_WIDTH / 2,
                                       ((double) screen->screen_offset.y + (double) screen->screen_height / 2) * 2},
                                       (Vector) {0, 0}, 0.0, 0.0, '#', sim);
            sim->following = sim->editedBody;
            break;
        case OPTION_DELETE_BODY:
            gui->editMenu_state = EDIT_MENU_STATE_DELETE_BODY;
            break;
        case OPTION_EDIT_BODY:
            gui->editMenu_state = EDIT_MENU_STATE_EDIT_BODY;
            break;
        case OPTION_FOLLOW_BODY:
            gui->editMenu_state = EDIT_MENU_STATE_FOLLOW_BODY;
            break;
        case OPTION_TOGGLE_DETAILS:
            li->infoLayer.enabled = !li->infoLayer.enabled;
            break;
        case OPTION_TOGGLE_G_RANGE:
            li->rangeLayer.enabled = !li->rangeLayer.enabled;
            break;
        case OPTION_TOGGLE_TRAILS:
            li->trailLayer.enabled = !li->trailLayer.enabled;
            break;
        case OPTION_EXPORT_SYSTEM:
            export_switchTo(program);
            break;
        case OPTION_EXIT:
            program->exiting = true;
            break;
        default:
            break;
    }

    if(lastState != gui->editMenu_state)
        gui->cursorPos = 0;
}


/** Enter on "Edit body"/[body] */
static void editMenu_selectEditOption(GUI *gui, Simulation *sim){
    if(gui->cursorPos == sim->bodyArray.length){
        gui->editMenu_state = EDIT_MENU_STATE_MAIN;
        gui->cursorPos = 0;
    } else{
        sim->editedBody = &sim->bodyArray.data[gui->cursorPos];
        sim->following = sim->editedBody;
        gui->editMenu_state = EDIT_MENU_STATE_EDIT_BODY_SET;
        gui->cursorPos = 0;
    }
}


/** Enter on "Edit body"/[body]/[property] */
static void editMenu_selectEditPropertyOption(GUI *gui, Program *program){
    if(gui->cursorPos == bodyEditableProperty_MAX){
        gui->cursorPos = 0;
        gui->editMenu_state = EDIT_MENU_STATE_EDIT_BODY;
    } else {
        gui->bodyEditor_state = gui->cursorPos;
        if(gui->cursorPos != BODY_SET_POS)
            bodyEditor_switchTo(program);
        else
            program->programState = PLACING_BODY;
    }
}


/** Enter on "Delete body"/[body] */
static void editMenu_selectDeleteOption(GUI *gui, Simulation *sim){
    if(gui->cursorPos == sim->bodyArray.length){
        gui->editMenu_state = EDIT_MENU_STATE_MAIN;
        gui->cursorPos = 0;
    } else {
        if(sim->following == &sim->bodyArray.data[gui->cursorPos])
            sim->following = NULL;
        bodyArray_removeAt(&sim->bodyArray, gui->cursorPos, sim);
        if(gui->cursorPos == sim->bodyArray.length && gui->cursorPos > 0)
            gui->cursorPos--;
    }
}


/** Enter on "Follow body"/[body] */
static void editMenu_selectFollowOption(GUI *gui, Simulation *sim){
    if(gui->cursorPos == sim->bodyArray.length){
        gui->editMenu_state = EDIT_MENU_STATE_MAIN;
        gui->cursorPos = 0;
    } else
        sim->following = &sim->bodyArray.data[gui->cursorPos];
}


/** Closes the edit menu and returns to simulation mode. */
static void editMenu_close(Program *program, GUI *gui, LayerInstances *li, Screen *screen, Simulation *sim){
    program->programState = SIMULATION;
    gui->editMenu_state = EDIT_MENU_STATE_MAIN;
    li->menuLayer.enabled = false;
    gui->cursorPos = 0;
    editMenu_render(li, screen, gui, sim);

    screen->screen_offset.x -= EDIT_MENU_WIDTH / 2;
}


// How many options should the given state render.
/*static int stateOptionCounts[EDIT_MENU_STATE_COUNT] = {EDIT_MENU_MAIN_OPTION_COUNT, BODY_EDITABLE_PROPERTY_COUNT + 1, -1, BODY_EDITABLE_PROPERTY_COUNT + 1,
                                                       -1, -1,}; // -1: count of bodies
*/

static int getOptionCount(EditMenuSTATE s){
    switch (s) {
        case EDIT_MENU_STATE_MAIN:
            return EditMenuMainOption_MAX;
        case EDIT_MENU_STATE_EDIT_BODY:
        case EDIT_MENU_STATE_DELETE_BODY:
        case EDIT_MENU_STATE_FOLLOW_BODY:
            return -1;
        case EDIT_MENU_STATE_ADD_BODY:
        case EDIT_MENU_STATE_EDIT_BODY_SET:
            return bodyEditableProperty_MAX + 1;
    }
}


void editMenu_processInput(Program *program, Simulation *sim, Screen *screen, GUI *gui, LayerInstances *li){
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        int cursorMax = getOptionCount(gui->editMenu_state);
        if(cursorMax < 0)
            cursorMax = sim->bodyArray.length + 1;

        if(key == KEY_ESCAPE || key =='e')
            editMenu_close(program, gui, li, screen, sim);
        else if ((key == 's' || key == KEY_DOWN) && gui->cursorPos < cursorMax - 1)
            gui->cursorPos++;
        else if ((key == 'w' || key == KEY_UP) && gui->cursorPos > 0)
            gui->cursorPos--;
        else if (key == KEY_ENTER || key == ' ') {
            switch (gui->editMenu_state) {
                case EDIT_MENU_STATE_MAIN:
                    editMenu_selectMainOption(gui, sim, screen, li, program);
                    break;
                case EDIT_MENU_STATE_EDIT_BODY:
                    editMenu_selectEditOption(gui, sim);
                    break;
                case EDIT_MENU_STATE_EDIT_BODY_SET:
                    editMenu_selectEditPropertyOption(gui, program);
                    break;
                case EDIT_MENU_STATE_DELETE_BODY:
                    editMenu_selectDeleteOption(gui, sim);
                    break;
                case EDIT_MENU_STATE_FOLLOW_BODY:
                    editMenu_selectFollowOption(gui, sim);
                    break;
                default:
                    break;
            }
        }
    }
}