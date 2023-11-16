#ifndef ASCII_SSS_STRUCTS_H
#define ASCII_SSS_STRUCTS_H


#include <stdbool.h>
#include <time.h>



//============= Program =================

typedef enum ProgramState{
    PROGRAM_STATE_EDIT_MENU,
    PROGRAM_STATE_SIMULATION,
    PROGRAM_STATE_TEXT_INPUT,
    PROGRAM_STATE_PLACING_BODY,
} ProgramState;

typedef enum TextInputDest{
    TEXT_INPUT_BODY_EDITOR,
    TEXT_INPUT_EXPORT,
    TEXT_INPUT_IMPORT,
} TextInputDest;

typedef struct Program{
    ProgramState state;
    TextInputDest textInputDest;
    double sleepTime;
    bool exiting;
} Program;



//============= Vector geometry =================

typedef struct Vector{
    double x;
    double y;
} Vector;

typedef struct Point{
    long long int x;
    long long int y;
} Point;



//============= Simulation =================

/** A point of a trail; a queue element. */
typedef struct Trail{
    Point position;
    struct Trail *next;
} Trail;

/** A queue of trail points. */
typedef struct TrailQueue{
    Trail *head;
    int length;
    int capacity;
} TrailQueue;


typedef enum BodyEditableProperty {
    BODY_PROPERTY_NAME,
    BODY_PROPERTY_MASS,
    BODY_PROPERTY_R,
    BODY_PROPERTY_POS,
    BODY_PROPERTY_VEL,
    bodyEditableProperty_MAX
} BodyEditableProperty;


/** A celestial body. */
typedef struct Body{
    char name[13];
    Vector position;
    Vector velocity;
    double r;
    double mass;
    char color;
    TrailQueue trail;
} Body;

typedef struct BodyArray{
    Body *data;
    int length;
    int capacity;
} BodyArray;

typedef struct Simulation{
    double solarMass;                   // The mass of the sun; every gravitational effect is relative to this!
    double detectCollisionPercentage;   // The percentage of the minimum radius overlap that triggers a collision event.
    bool fullSpeed;                     // Overrides the FPS limit -> the simulation will run at maximum speed
    bool pausedByUser;                  // Is the simulation pasued by the user
    int trailSpacingCounter;            // Measures the time passed since the last trail point.
    BodyArray bodyArray;

    Body *sun;
    Body *following;  // The body that the camera follows (NULL, if the camera is free).
    Body *editedBody; // The body that is currently being edited by the body editor.
} Simulation;



//============= Menu & GUI =================

typedef enum EditMenuMainOption {
    OPTION_ADD_BODY,
    OPTION_EDIT_BODY,
    OPTION_DELETE_BODY,
    OPTION_FOLLOW_BODY,
    OPTION_TOGGLE_DETAILS,
    OPTION_TOGGLE_G_RANGE,
    OPTION_TOGGLE_TRAILS,
    OPTION_IMPORT_SYSTEM,
    OPTION_EXPORT_SYSTEM,
    OPTION_EXIT,
    EditMenuMainOption_MAX
} EditMenuMainOption;


typedef enum EditMenuSTATE {
    EDIT_MENU_STATE_MAIN,
    EDIT_MENU_STATE_ADD_BODY,
    EDIT_MENU_STATE_EDIT_BODY,
    EDIT_MENU_STATE_EDIT_BODY_SET,
    EDIT_MENU_STATE_DELETE_BODY,
    EDIT_MENU_STATE_FOLLOW_BODY,
} EditMenuSTATE;

typedef enum BodyEditorState {
    BODY_SET_NAME,
    BODY_SET_MASS,
    BODY_SET_R,
    BODY_SET_POS,
    BODY_SET_V,
} BodyEditorState;

typedef struct Gui{
    EditMenuSTATE editMenu_state;
    BodyEditorState bodyEditor_state;
    int cursorPos; // The current position of the selection cursor
    Point textPos;
} Gui;



//============= Render =================

#define LAYER_COUNT 6

/** A 2D matrix that can be written onto the console. */
typedef struct Layer{
    char **text; // 2D array
    bool enabled;
} Layer;

typedef struct LayerInstances{
    Layer overlayLayer;
    Layer menuLayer;
    Layer infoLayer;
    Layer bodyLayer;
    Layer rangeLayer;
    Layer trailLayer;
} LayerInstances;

typedef struct LayerStatic{
    LayerInstances layerInstances;
    Layer *layers[LAYER_COUNT]; // The array of layers in order of priority (0. > 1. > ...)
} LayerStatic;

typedef struct Screen{
    int width;
    int height;
    char *buffer; // Used as the buffer of the console
    size_t bufferSize;
    Point offset;
    int fps;
    int targetFPS;
    time_t frameCountResetedTime; // The last time the FPS was evaluated
    int frameCount; // Frames since last reset (~1s)
} Screen;


#endif //ASCII_SSS_STRUCTS_H
