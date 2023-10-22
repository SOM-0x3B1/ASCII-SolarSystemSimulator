#include "global.h"


State currentState;

int screen_width;
int screen_height;
Point screen_offset = {0, 0};

int targetFPS;

double sleepTime = 0.01;

bool exiting = false;