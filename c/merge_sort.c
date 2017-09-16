#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int *merge_sort(int left[], int size)
{
    if (size <= 1) {
        return left;
    }

    // get all the elements to the right of middleId (inclusive)
    int *right = malloc(sizeof(int) * (size / 2 + 1)); 
    int middleId = (int)(size / 2); // the cast also floors the value when size is odd
    int i, j = 0;
    
    for (i = middleId; i < size; i++) {
        right[j] = left[i];
        j++;
    }
    
    unsigned int leftSize = middleId, rightSize = size - middleId;
    
    if (leftSize > 1) {
        left = merge_sort(left, leftSize);
    }
    if (rightSize > 1) {
        right = merge_sort(right, rightSize);
    }

    // sort content of left and right
    int *newArray = malloc(sizeof(int) * size);
    int leftId = 0, rightId = 0, arrayId = 0;

    while (leftId < leftSize && rightId < rightSize) {
        if (left[leftId] <= right[rightId]) {
            newArray[arrayId] = left[leftId];
            leftId++;
        } else {
            newArray[arrayId] = right[rightId];
            rightId++;
        }
        arrayId++;
    }

    // add the remaining values from left and right in the new sorted array
    // we know that by that time, the remaining values in left are all smaller than the ones in right
    for (i = leftId; i < leftSize; i++) {
        newArray[arrayId] = left[i];
        arrayId++;
    }
    for (i = rightId; i < rightSize; i++) {
        newArray[arrayId] = right[i];
        arrayId++;
    }
    free(left);
    free(right);

    return newArray;
}

int main(int argc, char *argv[]) 
{
    int arrayCount = atoi(argv[1]);
    int arraySize = atoi(argv[2]);
    
    int **data = malloc(sizeof(int) * arraySize *  arrayCount);
    int i, j, dataId = 0;

    for (i = 0; i < arrayCount; i++) { 
        int *array = malloc(sizeof(int) * arraySize);
        for (j = 0; j < arraySize; j++) { 
            array[j] = rand() % arraySize;
        }
        data[dataId] = array;
        dataId++;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (i = 0; i < arrayCount; i++) {
        merge_sort(data[i], arraySize);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double diffSec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;

    printf("Array count: %d \n", arrayCount);
    printf("Array size: %d \n", arraySize);
    printf("C:                      %f s\n", diffSec);

    return 0;
}
