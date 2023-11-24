#include <stdbool.h>
#include <stdio.h>
#include "body_editor.h"
#include "../graphics/drawing.h"
#include <string.h>


/** Returns the input promt text based on the bodyeditor's current state. */
static void getPrompt(char *res, BodyEditorState s){
    switch (s) {
        case BODY_SET_NAME:
            strcpy(res, "Name:");
            break;
        case BODY_SET_MASS:
            strcpy(res, "Mass (relative to Earth):");
            break;
        case BODY_SET_R:
            strcpy(res, "Radius:");
            break;
        default:
            strcpy(res, "Velocity:");
            break;
    }
}


void bodyEditor_switchTo(Program *p){
    p->state = PROGRAM_STATE_TEXT_INPUT;
    p->textInputDest = TEXT_INPUT_BODY_EDITOR;
}

void bodyEditor_render(Program *program, LayerInstances *li, Screen *screen, Gui *gui) {
    char sPrompt[30];
    getPrompt(sPrompt, gui->bodyEditor_state);

    if (program->state == PROGRAM_STATE_TEXT_INPUT)
        gui->textPos = drawing_drawInputPrompt(&li->menuLayer, screen->height / 4,
                                               "Body editor", sPrompt, screen);
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
    char temp;

    switch (gui->bodyEditor_state) {
        case BODY_SET_NAME: {
            char name[13];
            fgets(name, 12, stdin);
            name[12] = '\0';
            for (int i = 0; i < 12; ++i)
                if(name[i] == '\n')
                    name[i] = '\0';
            strcpy(sim->editedBody->name, name);
            //scanf("%12[^\n]s", sim->editedBody->name);
            if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY)
                gui->bodyEditor_state = BODY_SET_MASS;
            else
                program->state = PROGRAM_STATE_EDIT_MENU;
            break;
        }
        case BODY_SET_MASS:
            scanf("%31s", sValue1);
            if (sscanf(sValue1, "%lf", &value1) == 1 && value1 >= 0) {
                sim->editedBody->mass = value1;
                if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY)
                    gui->bodyEditor_state = BODY_SET_R;
                else
                    program->state = PROGRAM_STATE_EDIT_MENU;
            } else
                return ERR_ADDBODY_VALUE;
            scanf("%c", &temp);
            break;
        case BODY_SET_R:
            scanf("%31s", sValue1);
            if (sscanf(sValue1, "%lf", &value1) == 1 && value1 > 0) {
                sim->editedBody->r = value1;
                if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY) {
                    program->state = PROGRAM_STATE_PLACING_BODY;
                    gui->bodyEditor_state = BODY_SET_POS;
                } else
                    program->state = PROGRAM_STATE_EDIT_MENU;
            } else
                return ERR_ADDBODY_VALUE;
            scanf("%c", &temp);
            break;
        case BODY_SET_V:
            scanf("%31s %31s", sValue1, sValue2);
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
            scanf("%c", &temp);
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
    int key = 0;
    if (econio_kbhit()) {
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
                    sim->editedBody->trail.top->position = vector_toPoint(sim->editedBody->position);
            }
    }
}