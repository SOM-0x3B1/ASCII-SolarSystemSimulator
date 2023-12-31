#include "main_menu.h"


void mainMenu_render(Screen *screen, LayerInstances *li, Gui *gui) {
    for (int y = 0; y < EARTH_ANIMATION_HEIGHT; ++y)
        drawing_drawText(&li->menuLayer, screen->width - (EARTH_ANIMATION_WIDTH - 50),
                         y + (screen->height / 2 - EARTH_ANIMATION_HEIGHT / 2 - 2),
                         gui->mainMenu_animation[gui->mainMenu_animation_frameIndex][y], screen);
    gui->mainMenu_animation_frameIndex++;
    if (gui->mainMenu_animation_frameIndex >= EARTH_ANIMATION_FRAMES)
        gui->mainMenu_animation_frameIndex = 0;

    for (int y = 0; y < 13; ++y)
        drawing_drawText(&li->menuLayer, 12, y + screen->height / 6, gui->mainMenu_title[y], screen);

    drawing_drawText(&li->menuLayer, 18, screen->height / 4 * 3 + 2, "Press 'ENTER' to start the simulation", screen);
}

void mainMenu_startSim(Program *program, LayerInstances *li) {
    program->state = PROGRAM_STATE_SIMULATION;

    li->overlayLayer.enabled = true;
    li->menuLayer.enabled = false;
    li->infoLayer.enabled = true;
    li->bodyLayer.enabled = true;
    li->rangeLayer.enabled = true;
    li->trailLayer.enabled = true;
}

void mainMenu_processInput(Program *program, LayerInstances *li) {
    int key = 0;
    while (econio_kbhit())
        key = econio_getch();

    if (key == KEY_ENTER || key == ' ')
        mainMenu_startSim(program, li);
}