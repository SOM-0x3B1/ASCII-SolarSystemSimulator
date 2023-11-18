#include "edit_menu.h"
#include "../graphics/drawing.h"
#include "body_editor.h"
#include "../fs.h"


void editMenu_switchTo(EconioKey key, Program *program, Gui *gui, Screen *screen, LayerInstances *li){
    program->state = PROGRAM_STATE_EDIT_MENU;
    gui->editMenu_state = EDIT_MENU_STATE_MAIN;

    li->menuLayer.enabled = true;
    screen->offset.x += EDIT_MENU_WIDTH / 2;

    if(key == KEY_ESCAPE)
        gui->cursorPos = OPTION_EXIT;
}


/** Renders the "[ON]"/"[OFF]" texts next to toggle buttons. */
static void editMenu_renderToggleOptionSTATE(int y, bool stat, LayerInstances *li, Screen *screen){
    drawing_drawText(&li->menuLayer, screen->width - 13, y, stat ? "[ON] " : "[OFF]", screen);
}


/** Renders the '>' sign next to the selected option. */
static void editMenu_renderSelection(int i, int y, Gui *gui, LayerInstances *li, Screen *screen){
    if(i == gui->cursorPos)
        drawing_drawText(&li->menuLayer, screen->width - 30, y, ">", screen);
    else
        drawing_drawText(&li->menuLayer, screen->width - 30, y, " ", screen);
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
static void editMenu_renderMain(LayerInstances *li, Gui *gui, Screen *screen){
    int yOffset = 0;
    for (int i = 0; i < EditMenuMainOption_MAX; ++i) {
        if(i > OPTION_EXPORT_SYSTEM)
            yOffset = 3;
        else if(i > OPTION_TOGGLE_TRAILS)
            yOffset = 2;
        else if(i > OPTION_DELETE_BODY)
            yOffset = 1;

        int y = 5 + i + yOffset;

        drawing_drawText(&li->menuLayer, screen->width - 28, y, mainOptionToString(i), screen);
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
static void editMenu_renderBodyList(Simulation *sim, LayerInstances *li, Screen *screen, Gui *gui){
    for (int i = 0; i < sim->bodyArray.length; ++i) {
        int y = 5 + i;

        drawing_drawText(&li->menuLayer, screen->width - 28, y, sim->bodyArray.data[i].name, screen);
        editMenu_renderSelection(i, y, gui, li, screen);
    }

    drawing_drawText(&li->menuLayer, screen->width - 28, 5 + sim->bodyArray.length + 1, "Back", screen);
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
        case BODY_PROPERTY_VEL:
            return "Set velocity";
        default:
            return "";
    }
}


/** Renders the list of edit body properties. */
static void editMenu_renderEditProperties(LayerInstances *li, Screen *screen, Simulation *sim, Gui *gui){
    drawing_drawText(&li->menuLayer, screen->width - 30, 5, sim->editedBody->name, screen);

    for (int i = 0; i < bodyEditableProperty_MAX; ++i) {
        int y = 6 + i;

        drawing_drawText(&li->menuLayer, screen->width - 28, y, bodyOptionToString(i), screen);
        editMenu_renderSelection(i, y, gui, li, screen);
    }

    drawing_drawText(&li->menuLayer, screen->width - 28, 6 + bodyEditableProperty_MAX + 1, "Accept", screen);
    editMenu_renderSelection(bodyEditableProperty_MAX, 6 + bodyEditableProperty_MAX + 1, gui, li, screen);
}


void editMenu_render(LayerInstances *li, Screen *screen, Gui *gui, Simulation *sim){
    drawing_drawLine(&li->menuLayer, screen->width - 32, 2,
                     screen->height - 4, true, '|', screen);
    drawing_drawRectangleFill(&li->menuLayer, screen->width - 31, 2,
                              screen->width - 1, screen->height - 2, ' ', screen);
    drawing_drawText(&li->menuLayer, screen->width - 30, 3, "[EDIT MENU]", screen);

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


/** Enter on main menu option. */
static Error editMenu_selectMainOption(Gui *gui, Simulation *sim, Screen *screen, LayerInstances *li, Program *program){
    EditMenuSTATE lastState = gui->editMenu_state;

    switch (gui->cursorPos) {
        case OPTION_ADD_BODY:
            gui->editMenu_state = EDIT_MENU_STATE_ADD_BODY;
            gui->bodyEditor_state = BODY_SET_NAME;
            bodyEditor_switchTo(program);
            sim->editedBody =
                    body_new("",
                             (Vector) {(double) screen->offset.x + (double) screen->width / 2 - (double)EDIT_MENU_WIDTH / 2,
                                       ((double) screen->offset.y + (double) screen->height / 2) * 2},
                                       (Vector) {0, 0}, 0.0, 0.0, '#', sim);
            if(sim->editedBody == NULL) {
                program->exiting = true;
                return ERR_MEMORY;
            }
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

    return SUCCESS;
}


/** Enter on "Edit body"/[body] */
static void editMenu_selectEditOption(Gui *gui, Simulation *sim){
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
static void editMenu_selectEditPropertyOption(Gui *gui, Program *program){
    if(gui->cursorPos == bodyEditableProperty_MAX){
        gui->cursorPos = 0;
        gui->editMenu_state = EDIT_MENU_STATE_EDIT_BODY;
    } else {
        gui->bodyEditor_state = gui->cursorPos;
        if(gui->cursorPos != BODY_SET_POS)
            bodyEditor_switchTo(program);
        else
            program->state = PROGRAM_STATE_PLACING_BODY;
    }
}


/** Enter on "Delete body"/[body] */
static void editMenu_selectDeleteOption(Gui *gui, Simulation *sim){
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
static void editMenu_selectFollowOption(Gui *gui, Simulation *sim){
    if(gui->cursorPos == sim->bodyArray.length){
        gui->editMenu_state = EDIT_MENU_STATE_MAIN;
        gui->cursorPos = 0;
    } else
        sim->following = &sim->bodyArray.data[gui->cursorPos];
}


/** Closes the edit menu and returns to simulation mode. */
static void editMenu_close(Program *program, Gui *gui, LayerInstances *li, Screen *screen, Simulation *sim){
    program->state = PROGRAM_STATE_SIMULATION;
    gui->editMenu_state = EDIT_MENU_STATE_MAIN;
    li->menuLayer.enabled = false;
    gui->cursorPos = 0;
    editMenu_render(li, screen, gui, sim);

    screen->offset.x -= EDIT_MENU_WIDTH / 2;
}


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


Error editMenu_processInput(Program *program, Simulation *sim, Screen *screen, Gui *gui, LayerInstances *li){
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
                    return editMenu_selectMainOption(gui, sim, screen, li, program);
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
    return SUCCESS;
}