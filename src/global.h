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
extern TextInputDest textInputDest; // Which module requested the text input.

extern int screen_width;
extern int screen_height;
extern Point screen_offset; // The top-left position of the camera.

extern int fps; // Current FPS
extern int targetFPS; // The program will try to maintain this value by adjusting the sleepTime variable.
extern double sleepTime; // Maintains the speed (and FPS) of the simulation.
extern bool fullSpeed; // Overrides the sleepTime -> speed and FPS will become unlimited.


extern bool pausedByUser;

extern bool exiting;


#endif //ASCII_SSS_GLOBAL_H