#ifndef ASCII_SSS_EDIT_MENU_H
#define ASCII_SSS_EDIT_MENU_H


#include "../lib/econio.h"


#define EDIT_MENU_WIDTH 32


typedef enum EditMenuSTATE {
    STATE_MAIN,
    STATE_ADD_BODY,
    STATE_EDIT_BODY,
    STATE_EDIT_BODY_SET,
    STATE_DELETE_BODY,
    STATE_FOLLOW_BODY,
} EditMenuSTATE;


extern EditMenuSTATE editMenu_state;


/**
 * Set the proper states and display parameters for edit menu.
 * @param key if it's Esc, the cursor jumps to the Exit function
 */
void editMenu_switchTo(EconioKey key);

/** Renders all edit menu components. */
void editMenu_render();

/** Reads raw input. */
void editMenu_processInput();


#endif //ASCII_SSS_EDIT_MENU_H