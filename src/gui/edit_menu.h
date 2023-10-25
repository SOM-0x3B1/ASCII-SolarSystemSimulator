#ifndef ASCII_SSS_EDIT_MENU_H
#define ASCII_SSS_EDIT_MENU_H


#include "../lib/econio.h"


typedef enum EditMenuSTATE {
    STATE_MAIN,
    STATE_ADD_BODY,
    STATE_EDIT_BODY,
    STATE_EDIT_BODY_SET,
    STATE_DELETE_BODY,
    STATE_FOLLOW_BODY,
} EditMenuSTATE;


extern EditMenuSTATE editMenu_state;


void editMenu_switchTo(EconioKey key);

void editMenu_render();

void editMenu_processInput();

#endif //ASCII_SSS_EDIT_MENU_H