#include <stdbool.h>
#include <stdio.h>
#include "body_editor.h"
#include "../graphics/drawing.h"


/** Returns the input promt text based on the bodyeditor's current state. */
static char *getPrompt(BodyEditorState s){
    switch (s) {
        case BODY_SET_NAME:
            return "Name:";
        case BODY_SET_MASS:
            return "Mass (relative to Earth):";
        case BODY_SET_R:
            return "Radius:";
        default:
            return "Velocity:";
    }
}


void bodyEditor_switchTo(Program *p){
    p->state = PROGRAM_STATE_TEXT_INPUT;
    p->textInputDest = TEXT_INPUT_BODY_EDITOR;
}

void bodyEditor_render(Program *program, LayerInstances *li, Screen *screen, Gui *gui) {
    if (program->state == PROGRAM_STATE_TEXT_INPUT)
        gui->textPos = drawing_drawInputPrompt(&li->menuLayer, screen->height / 4,
                                               "Body editor", getPrompt(gui->bodyEditor_state), screen);
    else {
        int xCentrer = drawing_drawBox(&li->menuLayer, 2, 2, 43, 8, "Placing body", screen);
        drawing_drawText(&li->menuLayer, xCentrer - 14, 6, "Use 'WASD' to move the body", screen);
        drawing_drawText(&li->menuLayer, xCentrer - 12, 7, "Press 'ENTER' to accept", screen);
    }
}

Error bodyEditor_processTextInput(Program *program, Gui *gui, Simulation *sim) {
    econio_gotoxy((int) gui->textPos.x, (int)gui->textPos.y);
    econio_normalmode();

    char sValue1[32];
    char sValue2[32];
    double value1;
    double value2;

    switch (gui->bodyEditor_state) {
        case BODY_SET_NAME:
            if (scanf("%s", sim->editedBody->name) == 1) {
                if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY)
                    gui->bodyEditor_state = BODY_SET_MASS;
                else
                    program->state = PROGRAM_STATE_EDIT_MENU;
            } else
                return ERR_ADDBODY_VALUE;
            break;
        case BODY_SET_MASS:
            scanf("%s", sValue1);
            if (sscanf(sValue1, "%lf", &value1) == 1 && value1 >= 0) {
                sim->editedBody->mass = value1;
                if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY)
                    gui->bodyEditor_state = BODY_SET_R;
                else
                    program->state = PROGRAM_STATE_EDIT_MENU;
            } else
                return ERR_ADDBODY_VALUE;
            break;
        case BODY_SET_R:
            scanf("%s", sValue1);
            if (sscanf(sValue1, "%lf", &value1) == 1 && value1 > 0) {
                sim->editedBody->r = value1;
                if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY) {
                    program->state = PROGRAM_STATE_PLACING_BODY;
                    gui->bodyEditor_state = BODY_SET_POS;
                } else
                    program->state = PROGRAM_STATE_EDIT_MENU;
            } else
                return ERR_ADDBODY_VALUE;
            break;
        case BODY_SET_V:
            scanf("%s %s", sValue1, sValue2);
            if (sscanf(sValue1, "%lf", &value1) == 1 && sscanf(sValue2, "%lf", &value2) == 1) {
                sim->editedBody->velocity.x = value1;
                sim->editedBody->velocity.y = value2;
                sim->editedBody->velocity.y = -sim->editedBody->velocity.y;
                if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY) {
                    program->state = PROGRAM_STATE_EDIT_MENU;
                    gui->editMenu_state = EDIT_MENU_STATE_EDIT_BODY_SET;
                } else
                    program->state = PROGRAM_STATE_EDIT_MENU;
            } else
                return ERR_ADDBODY_VALUE;
            break;
        default:
            break;
    }
    econio_rawmode();
    econio_gotoxy(0, 0);

    return SUCCESS;
}


bool bodyEditor_moveBody(EconioKey key, Simulation *sim){
    if (key == 's' || key == KEY_DOWN) {
        sim->editedBody->position.y++;
        return true;
    }
    else if (key == 'w' || key == KEY_UP) {
        sim->editedBody->position.y--;
        return true;
    }
    else if (key == 'a' || key == KEY_LEFT) {
        sim->editedBody->position.x -= 2;
        return true;
    }
    else if (key == 'd' || key == KEY_RIGHT) {
        sim->editedBody->position.x += 2;
        return true;
    }
    return false;
}

void bodyEditor_processPlacementInput(Program *program, Gui *gui, Simulation *sim){
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        if (key == KEY_ENTER)
        {
            if(gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY) {
                program->state = PROGRAM_STATE_TEXT_INPUT;
                gui->bodyEditor_state = BODY_SET_V;
            } else
                program->state = PROGRAM_STATE_EDIT_MENU;
        }
        else
            if(bodyEditor_moveBody(key, sim)) {
                if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY)
                    sim->editedBody->trail.head->position = vector_toPoint(sim->editedBody->position);
            }
    }
}