#include <stdbool.h>
#include <stdio.h>
#include "body_editor.h"
#include "../graphics/drawing.h"
#include "edit_menu.h"


//BodyEditorState bodyEditor_state = BODY_SET_NAME;


/*static Point textPos; // The position of the cursor on an input text prompt

// The prompt strings stored for each state
static const char *prompts[5] = {, "Mass (relative to Earth):",
                           "Size (in character radius):", "Position:",
                           "Velocity (format: \"x y\"):"};*/

static char *getPropt(BodyEditorState s){
    switch (s) {
        case BODY_SET_NAME:
            return "Name:";
        case BODY_SET_MASS:
            return "Mass (relative to Earth):";
        case BODY_SET_SIZE:
            return "Radius:";
        default:
            return "Velocity:";
    }
}


void bodyEditor_switchTo(Program *p){
    p->programState = TEXT_INPUT;
    p->textInputDest = TEXT_INPUT_BODY_EDITOR;
}

void bodyEditor_render(Program *program, LayerInstances *li, Screen *screen, GUI *gui) {
    if (program->programState == TEXT_INPUT)
        gui->textPos = drawing_drawInputPrompt(&li->menuLayer, screen->screen_height / 4,
                                          "Body editor", getPropt(gui->bodyEditor_state), screen);
    else {
        int xCentrer = drawing_drawBox(&li->menuLayer, 2, 2, 43, 8, "Placing body", screen);
        drawing_drawText(&li->menuLayer, xCentrer - 14, 6, "Use 'WASD' to move the body", screen);
        drawing_drawText(&li->menuLayer, xCentrer - 12, 7, "Press 'ENTER' to accept", screen);
    }
}

void bodyEditor_processTextInput(Program *program, GUI *gui, Simulation *sim) {
    econio_gotoxy((int) gui->textPos.x, (int)gui->textPos.y);
    econio_normalmode();
    switch (gui->bodyEditor_state) {
        case BODY_SET_NAME:
            scanf("%s", sim->editedBody->name);
            if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY)
                gui->bodyEditor_state = BODY_SET_MASS;
            else
                program->programState = EDIT_MENU;
            break;
        case BODY_SET_MASS:
            scanf("%lf", &sim->editedBody->mass);
            if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY)
                gui->bodyEditor_state = BODY_SET_SIZE;
            else
                program->programState = EDIT_MENU;
            break;
        case BODY_SET_SIZE:
            scanf("%lf", &sim->editedBody->r);
            if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY) {
                program->programState = PLACING_BODY;
                gui->bodyEditor_state = BODY_SET_POS;
            } else
                program->programState = EDIT_MENU;
            break;
        case BODY_SET_V:
            scanf("%lf %lf", &sim->editedBody->velocity.x, &sim->editedBody->velocity.y);
            sim->editedBody->velocity.y = -sim->editedBody->velocity.y;
            if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY) {
                program->programState = EDIT_MENU;
                gui->editMenu_state = EDIT_MENU_STATE_EDIT_BODY_SET;
            } else
                program->programState = EDIT_MENU;
            break;
        default:
            break;
    }
    econio_rawmode();
    econio_gotoxy(0, 0);
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

void bodyEditor_processPlacementInput(Program *program, GUI *gui, Simulation *sim){
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        if (key == KEY_ENTER)
        {
            if(gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY) {
                program->programState = TEXT_INPUT;
                gui->bodyEditor_state = BODY_SET_V;
            } else
                program->programState = EDIT_MENU;
        }
        else
            if(bodyEditor_moveBody(key, sim)) {
                if (gui->editMenu_state == EDIT_MENU_STATE_ADD_BODY)
                    sim->editedBody->trail.head->position = vector_toPoint(sim->editedBody->position);
            }
    }
}