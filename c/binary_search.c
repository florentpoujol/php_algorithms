#include <stdlib.h>
#include <stdio.h>
#include "utils.c"


int binary_search_rec(int *array, const int size, const int target)
{  
    if (size <= 0) {
        return 0;
    }

    if (size == 1) {
        return (target == array[0]);
    }

    const int middleId = (int)(size / 2);
    int *pmiddle = (array + middleId);
    
    if (target == *pmiddle) {
        return 1;
    }

    if (target < *pmiddle) {
        return binary_search_rec(array, middleId, target);
    }

    return binary_search_rec(pmiddle + 1, size - middleId - 1, target);    
}


int binary_search_loop(int *start, int size, const int target)
{  
    int *pmiddle = 0;

    while (size > 0) {
        if (size == 1) {
            return (target == *start);
        }

        int middleId = (int)(size / 2);
        pmiddle = (start + middleId);
        
        if (target == *pmiddle) {
            return 1;
        }

        if (target < *pmiddle) {
            size = middleId;
        } else {
            start = pmiddle + 1;
            size -= middleId - 1;
        }
    }

    return 0;
}


int main(int argc, char *argv[]) 
{
    int arrayCount = atoi(argv[1]);
    int arraySize = atoi(argv[2]);
    
    int *array = malloc(sizeof(int) * arraySize);
    int *targets = malloc(sizeof(int) * arrayCount);
    
    srand(time(NULL));
    rand(); rand(); rand();
    int i;

    for (i = 0; i < arraySize; i++) {
        array[i] = i;
    }
    for (i = 0; i < arrayCount; i++) {
        targets[i] = rand() % arraySize;
    }

    REGISTER_TIME(start, "start");

    for (i = 0; i < arrayCount; i++) {
        if (! binary_search_rec(array, arraySize, targets[i])) {
            printf("wrong rec target=%d \n", targets[i]);
            // array_print_inline(array, arraySize, "");
        }
        // binary_search((array + i), arraySize, targets[i]);
    }

    REGISTER_TIME(rec, "rec");

    for (i = 0; i < arrayCount; i++) {
        if (! binary_search_loop(array, arraySize, targets[i])) {
            printf("wrong loop target=%d \n", targets[i]);
            // array_print_inline(array, arraySize, "");
        }
        // binary_search((array + i), arraySize, targets[i]);
    }

    REGISTER_TIME(loop, "loop");

    double recDiff = getDiff(start, rec);
    double loopDiff = getDiff(rec, loop);

    printf("Array count: %d \n", arrayCount);
    printf("Array size: %d \n", arraySize);
    printf("C (rec):         %f s\n", recDiff);
    printf("C (loop):        %f s\n", loopDiff);

    return 0;
}

