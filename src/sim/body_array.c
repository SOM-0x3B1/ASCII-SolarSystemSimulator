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
    free(bodyArray.data);
}

Body *bodyArray_add(Body *b){
    if(bodyArray.length + 1 > bodyArray.capacity) {
        Body *newArray = realloc(bodyArray.data, bodyArray.capacity * 2 * sizeof(Body));
        if(newArray == NULL)
            return NULL;  // failed to allocate memory for body
        else {
            bodyArray.data = newArray;
            bodyArray.capacity *= 2;
        }
    }

    bodyArray.data[bodyArray.length] = *b;
    bodyArray.length++;

    return &bodyArray.data[bodyArray.length - 1];
}

void bodyArray_removeAt(int i){
    for (int j = i; j < bodyArray.length - 1; ++j)
        bodyArray.data[j] = bodyArray.data[j + 1];

    bodyArray.length--;
}