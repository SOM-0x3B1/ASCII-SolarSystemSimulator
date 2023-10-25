#ifndef ASCII_SSS_GLOBAL_H
#define ASCII_SSS_GLOBAL_H

#include <stdbool.h>
#include "vector.h"


typedef enum State{
    EDIT_MENU,
    SIMULATION,
    TEXT_INPUT,
    PLACING_BODY,
} State;


extern State programState;

extern int screen_width;
extern int screen_height;
extern Point screen_offset;

extern int targetFPS;
extern double sleepTime;

extern bool exiting;


#endif //ASCII_SSS_GLOBAL_H