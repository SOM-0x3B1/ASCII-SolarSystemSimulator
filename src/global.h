#ifndef ASCII_SSS_GLOBAL_H
#define ASCII_SSS_GLOBAL_H

#include <stdbool.h>
#include "vector.h"


#define PROGRAM_STATE_COUNT 4


typedef enum {
    EDIT_MENU,
    SIMULATION,
    TEXT_INPUT,
    PLACING_BODY,
} ProgramState;

typedef enum {
    TEXT_INPUT_BODY_EDITOR,
    TEXT_INPUT_EXPORT,
    TEXT_INPUT_IMPORT,
} TextInputDest;


extern ProgramState programState;
extern TextInputDest textInputDest;

extern int screen_width;
extern int screen_height;
extern Point screen_offset;

extern int fps;
extern int targetFPS;
extern double sleepTime;
extern bool fullSpeed;


extern bool pausedByUser;

extern bool exiting;


#endif //ASCII_SSS_GLOBAL_H