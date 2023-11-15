#ifndef ASCII_SSS_STRUCTS_H
#define ASCII_SSS_STRUCTS_H


#include <stdbool.h>
#include <time.h>


typedef enum ProgramState{
    EDIT_MENU,
    SIMULATION,
    TEXT_INPUT,
    PLACING_BODY,
} ProgramState;

typedef enum TextInputDest{
    TEXT_INPUT_BODY_EDITOR,
    TEXT_INPUT_EXPORT,
    TEXT_INPUT_IMPORT,
} TextInputDest;

typedef struct Program{
    ProgramState programState;
    TextInputDest textInputDest;
    double sleepTime;
    bool exiting;
} Program;

typedef struct Vector{
    double x;
    double y;
} Vector;

typedef struct Point{
    long long int x;
    long long int y;
} Point;

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
    int trail_spacing_counter;          // Measures the time passed since the last trail point.
    BodyArray bodyArray;

    Body *sun;
    Body *following;  // The body that the camera follows (NULL, if the camera is free).
    Body *editedBody; // The body that is currently being edited by the body editor.
} Simulation;


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
    BODY_SET_SIZE,
    BODY_SET_POS,
    BODY_SET_V,
} BodyEditorState;


typedef struct GUI{
    EditMenuSTATE editMenu_state;
    BodyEditorState bodyEditor_state;
    int cursorPos; // The current position of the selection cursor
    Point textPos;
} GUI;

typedef struct Screen{
    int screen_width;
    int screen_height;
    char *screenBuffer; // Used as the buffer of the console
    Point screen_offset;
    int fps;
    int targetFPS;
    time_t frameCountResetedTime; // The last time the FPS was evaluated
    int frameCount; // Frames since last reset (~1s)
} Screen;

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


#define LAYER_COUNT 6

typedef struct LayerProperties{
    LayerInstances layerInstances;
    Layer *layers[LAYER_COUNT]; // The array of layers in order of priority (0. > 1. > ...)
} LayerProperties;


#endif //ASCII_SSS_STRUCTS_H
