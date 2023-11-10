#ifndef ASCII_SSS_DRAWING_H
#define ASCII_SSS_DRAWING_H


#include "layer.h"
#include "../lib/econio.h"
#include "../vector.h"


/**
 * Writes a string onto a layer.
 * @param l Layer
 * @param s string
 */
void drawing_drawText(Layer *l, int x, int y, char const *s);

/**
 * Draws a line.
 * @param l Layer
 * @param length The length of the line
 * @param vertical Is the line vertical
 * @param c The characters of which the line should be built of
 */
void drawing_drawLine(Layer *l, int x, int y, int length, bool vertical, char c);


/**
 * Draws a filled rectangle without outline.
 * @param l Layer
 * @param x1 Top left (x)
 * @param y1 Top left (y)
 * @param x2 Bottom right (x)
 * @param y2 Bottom right (y)
 * @param c Fill character
 */
void drawing_drawRectangle(Layer *l, int x1, int y1, int x2, int y2, char c);

/**
 * Draws a window (box) with a title.
 * @param l Layer
 * @param x1 Top left (x)
 * @param y1 Top left (y)
 * @param x2 Bottom right (x)
 * @param y2 Bottom right (y)
 * @return The horizontal (x) position of the center of the box
 */
int drawing_drawBox(Layer *l, int x1, int y1, int x2, int y2, const char* title);

/**
 * Draws a text input box.
 * @param l Layer
 * @param yCenter The vertical center of the box
 * @param title The title of the box
 * @param text The prompt text
 * @return The coords of the cursor (next to the prompt text)
 */
Point drawing_drawInputPrompt(Layer *l, int yCenter, const char *title, const char *text);


#endif //ASCII_SSS_DRAWING_H
