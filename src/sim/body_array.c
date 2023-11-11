#include <stdlib.h>
#include "body_array.h"
#include "../lib/debugmalloc.h"


BodyArray bodyArray;


int bodyArray_init(){
    bodyArray.length = 0;
    bodyArray.capacity = 1;
    bodyArray.data = (Body*) malloc(sizeof(Body));
    if(bodyArray.data == NULL)
        return 1;
    return 0;
}
void bodyArray_dispose(){
    for (int i = 0; i < bodyArray.length; ++i)
        trailQueue_clear(&bodyArray.data[i].trail);
    free(bodyArray.data);
}

/**
 * If the body array shifts or gets relocated, the global body variables must be updated to their new pointers.
 * This function checks and updates a single body pointer.
 * @param array The new/shifted body array
 * @param origin Original index of the body.
 * @param newIndex The new index of the body.
 */
static void updatePointers(Body *array, int origin, int newIndex){
    if(&bodyArray.data[origin] == following)
        following = &array[newIndex];
    if(&bodyArray.data[origin] == sun)
        sun = &array[newIndex];
    if(&bodyArray.data[origin] == editedBody)
        editedBody = &array[newIndex];
}

Body *bodyArray_add(Body *b){
    if(bodyArray.length + 1 > bodyArray.capacity) {
        //Body *newArray = realloc(bodyArray.data, bodyArray.capacity * 2 * sizeof(Body));
        Body *newArray = (Body *) malloc(bodyArray.capacity * 2 * sizeof(Body));
        if(newArray == NULL)
            return NULL;  // failed to allocate memory for body
        else {
            for (int i = 0; i < bodyArray.length; ++i) {
                newArray[i] = bodyArray.data[i];
                updatePointers(newArray, i , i);
            }
            free(bodyArray.data);
            bodyArray.data = newArray;
            bodyArray.capacity *= 2;
        }
    }

    bodyArray.data[bodyArray.length] = *b;
    bodyArray.length++;

    return &bodyArray.data[bodyArray.length - 1];
}

void bodyArray_removeAt(int i) {
    if (following == &bodyArray.data[i])
        following = NULL;

    trailQueue_clear(&bodyArray.data[i].trail);

    for (int j = i; j < bodyArray.length - 1; ++j) {
        bodyArray.data[j] = bodyArray.data[j + 1];
        updatePointers(bodyArray.data, j+1, j);
    }
    bodyArray.length--;
}

void bodyArray_remove(Body *b){
    int i = 0;
    while (&bodyArray.data[i] != b && i < bodyArray.length)
        i++;
    if(i < bodyArray.length)
        bodyArray_removeAt(i);
}