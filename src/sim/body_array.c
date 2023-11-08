#include <stdlib.h>
#include "body_array.h"


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
        trailQueue_clear(bodyArray.data[i].trail);
    free(bodyArray.data);
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
                if(&bodyArray.data[i] == following)
                    following = &newArray[i];
            }
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

    trailQueue_clear(bodyArray.data[i].trail);

    for (int j = i; j < bodyArray.length - 1; ++j) {
        bodyArray.data[j] = bodyArray.data[j + 1];
        if (following == &bodyArray.data[j + 1])
            following = &bodyArray.data[j];
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