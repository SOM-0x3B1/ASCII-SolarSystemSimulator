#include <stdbool.h>
#include <stdio.h>
#include "body_editor.h"
#include "../graphics/drawing.h"
#include "../global.h"
#include <string.h>
#include "edit_menu.h"


BodyEditorOptions bodyEditor_state = BODY_SET_NAME;


//bool bodyEditor_enabled = false;
static Point textPos;

static Layer *ml;

static char *prompts[5] = {"Name:", "Mass (relative to Earth):",
                           "Size (in character radius):", "Position:",
                           "Velocity (format: \"x y\"):"};


/*static bool isStateTextInput(){
    return bodyEditor_state == BODY_SET_NAME || bodyEditor_state == BODY_SET_MASS || bodyEditor_state == BODY_SET_SIZE || bodyEditor_state == BODY_SET_V;
}*/

void bodyEditor_init(){
    ml = &menuLayer;
}

void bodyEditor_setStates(){
    programState = TEXT_INPUT;
    textInputDest = TEXT_INPUT_BODY_EDITOR;
}

void bodyEditor_render() {
    if (programState == TEXT_INPUT)
        textPos = drawing_drawInputPrompt(ml, screen_height / 4,
                                          "Body editor", prompts[bodyEditor_state]);
    else {
        int xCentrer = drawing_drawBox(ml, 2, 2, 43, 8, "Placing body");
        drawing_drawText(ml, xCentrer - 14, 6, "Use 'WASD' to move the body");
        drawing_drawText(ml, xCentrer - 12, 7, "Press 'ENTER' to accept");
    }
}

void bodyEditor_processTextInput() {
    econio_gotoxy((int) textPos.x, (int) textPos.y);
    econio_normalmode();
    switch (bodyEditor_state) {
        case BODY_SET_NAME:
            scanf("%s", editedBody->name);
            if (editMenu_state == STATE_ADD_BODY)
                bodyEditor_state = BODY_SET_MASS;
            else
                programState = EDIT_MENU;
            break;
        case BODY_SET_MASS:
            scanf("%lf", &editedBody->mass);
            if (editMenu_state == STATE_ADD_BODY)
                bodyEditor_state = BODY_SET_SIZE;
            else
                programState = EDIT_MENU;
            break;
        case BODY_SET_SIZE:
            scanf("%lf", &editedBody->r);
            if (editMenu_state == STATE_ADD_BODY) {
                programState = PLACING_BODY;
                bodyEditor_state = BODY_SET_POS;
            } else
                programState = EDIT_MENU;
            break;
        case BODY_SET_V:
            scanf("%lf %lf", &editedBody->velocity.x, &editedBody->velocity.y);
            editedBody->velocity.y = -editedBody->velocity.y;
            if (editMenu_state == STATE_ADD_BODY) {
                programState = EDIT_MENU;
                editMenu_state = STATE_EDIT_BODY_SET;
            } else
                programState = EDIT_MENU;
            break;
        default:
            break;
    }
    econio_rawmode();
    econio_gotoxy(0, 0);
}


bool bodyEditor_moveBody(EconioKey key){
    if (key == 's' || key == KEY_DOWN) {
        editedBody->position.y++;
        return true;
    }
    else if (key == 'w' || key == KEY_UP) {
        editedBody->position.y--;
        return true;
    }
    else if (key == 'a' || key == KEY_LEFT) {
        editedBody->position.x -= 2;
        return true;
    }
    else if (key == 'd' || key == KEY_RIGHT) {
        editedBody->position.x += 2;
        return true;
    }
    return false;
}

void bodyEditor_processPlacementInput(){
    if (econio_kbhit()) {
        int key;
        while (econio_kbhit())
            key = econio_getch();

        if (key == KEY_ENTER)
        {
            if(editMenu_state == STATE_ADD_BODY) {
                programState = TEXT_INPUT;
                bodyEditor_state = BODY_SET_V;
            } else
                programState = EDIT_MENU;
        }
        else
            bodyEditor_moveBody(key);
    }
}