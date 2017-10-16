#include <stdlib.h>
#include <stdio.h>
#include "utils.c"


int binary_search(int *array, const int size, const int target)
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
        return binary_search(array, middleId, target);
    }

    return binary_search(pmiddle + 1, size - middleId - 1, target);    
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
        if (! binary_search(array, arraySize, targets[i])) {
            printf("wrong 1 target=%d \n", targets[i]);
            // array_print_inline(array, arraySize, "");
        }
        // binary_search((array + i), arraySize, targets[i]);
    }

    REGISTER_TIME(bs1, "bs1");

    for (i = 0; i < arrayCount; i++) {
        if (! binary_search_loop(array, arraySize, targets[i])) {
            printf("wrong 2 target=%d \n", targets[i]);
            // array_print_inline(array, arraySize, "");
        }
        // binary_search((array + i), arraySize, targets[i]);
    }

    REGISTER_TIME(bs2, "bs2");

    double bs1Diff = getDiff(start, bs1);
    double bs2Diff = getDiff(bs1, bs2);

    printf("Array count: %d \n", arrayCount);
    printf("Array size: %d \n", arraySize);
    printf("C BS1:                  %f s\n", bs1Diff);
    printf("C BS2:                  %f s\n", bs2Diff);

    return 0;
}

