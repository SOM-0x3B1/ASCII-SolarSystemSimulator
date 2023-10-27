#include "global.h"


ProgramState programState;

int screen_width;
int screen_height;
Point screen_offset = {0, 0};

int fps;
int targetFPS;
double sleepTime = 0.01;
bool useLegacyRendering;

bool pausedByUser = false;

bool exiting = false;