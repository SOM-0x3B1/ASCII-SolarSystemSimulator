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


extern Body *editedBody;

extern BodyEditorOptions bodyEditor_state;


void bodyEditor_init();

void bodyEditor_setStates();

void bodyEditor_render();

void bodyEditor_processTextInput();

void bodyEditor_processPlacementInput();


#endif //ASCII_SSS_BODY_EDITOR_H
