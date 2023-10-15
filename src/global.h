#ifndef ASCII_SSS_GLOBAL_H
#define ASCII_SSS_GLOBAL_H

#endif //ASCII_SSS_GLOBAL_H

#include <stdbool.h>


typedef enum State{
    EDIT_MENU,
    SIMULATION
} State;

extern State currentState;

extern int screen_width;
extern int screen_height;

extern bool exiting;