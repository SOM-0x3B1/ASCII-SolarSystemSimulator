#include "global.h"


ProgramState programState;
TextInputDest textInputDest;


int screen_width;
int screen_height;
Point screen_offset = {0, 0};

int fps;
int targetFPS;
double sleepTime = 0.02;

bool pausedByUser = false;

bool exiting = false;