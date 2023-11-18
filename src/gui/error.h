#ifndef ASCII_SSS_ERROR_H
#define ASCII_SSS_ERROR_H


#include "../structs.h"


/** Renders an error message. */
void error_render(Error error, Screen *screen, LayerInstances *li);

/** Pauses the program until ENTER is pressed. */
void error_awaitConfirmation();


#endif //ASCII_SSS_ERROR_H
