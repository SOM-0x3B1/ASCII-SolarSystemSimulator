#ifndef ASCII_SSS_BODY_EDITOR_H
#define ASCII_SSS_BODY_EDITOR_H

#include "../sim/body.h"


typedef enum BodyEditorOptions {
    BODY_SET_NAME,
    BODY_SET_MASS,
    BODY_SET_SIZE,
    BODY_SET_POS,
    BODY_SET_V,
} BodyEditorOptions;


extern BodyEditorOptions bodyEditor_state;


/** Set the proper states to read text input for the body editor. */
void bodyEditor_switchTo();

/** Renders the current body editor GUI element.*/
void bodyEditor_render();

/** Processes the input text after pressing enter.*/
void bodyEditor_processTextInput();

/** Moves and places the edited body on the simulation window. */
void bodyEditor_processPlacementInput();


#endif //ASCII_SSS_BODY_EDITOR_H
