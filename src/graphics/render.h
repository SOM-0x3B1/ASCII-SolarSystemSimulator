#ifndef ASCII_SSS_RENDER_H
#define ASCII_SSS_RENDER_H

/**
 * Allocates memory for the screenbuffer.
 * @return failure
 */
bool render_init();
/** Frees the memory of the screenbuffer. */
void render_dispose();

/** Prints the content of the layers on the console. */
void render_refreshScreen();

/** Updates all the layers. */
void render_fullRender();


#endif //ASCII_SSS_RENDER_H