#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void display(int array[], int size, char *text)
{
    int i;
    printf("%s size=%d : ", text, size);
    for (i = 0; i < size; i++) {

        printf("%d ", array[i]);
    }
    printf("\n");
}


void quick_sort(int *array, const uint size)
{
    if (size <= 1) {
        return;
    }

    // select pivot (median of three)
    int first = array[0];
    int last = array[size - 1];
    if (first > last) {
        int temp = first;
        first = last;
        last = temp;
    }

    if (size == 2) {
        array[0] = first;
        array[1] = last;
        return;
    }

    int pivot = array[ (int)(size/2) ];
    if (pivot < first) {
        pivot = first;
    } else if (pivot > last) {
        pivot = last;
    }

    // moves values
    // could improve memory consumption by counting the values first
    // so that to create left and right with just the correct size
    // but it's 50% slower...
    // maybe we could realloc by chunk of 1/4 size ony when we need it...
    int *left  = malloc(sizeof(int) * size);
    int *right = malloc(sizeof(int) * size);

    int i, leftSize = 0, middleSize = 0, rightSize = 0;
    for (i = 0; i < size; i++) {
        int value = array[i];
        if (value < pivot) {
            left[ leftSize++ ] = value;
        } else if (value > pivot) {
            right[ rightSize++ ] = value;
        } else {
            middleSize++;
        }        
    }

    if (leftSize > 1) {
        quick_sort(left, leftSize);
    }

    if (rightSize > 1) {
        quick_sort(right, rightSize);
    }

    // merge
    for (i = 0; i < leftSize; i++) {
        array[i] = left[i];
    }
    for (i = 0; i < middleSize; i++) {
        array[ (leftSize + i) ] = pivot;
    }
    const int offset = leftSize + middleSize;
    for (i = 0; i < rightSize; i++) {
        array[ (offset + i) ] = right[i];
    }

    free(left);
    free(right);
}


int check(int *array, int size)
{
    for (int i=1; i < size; i++) { 
        if (array[i-1] > array[i]) {
            return 0;
        }
    }
    return 1;
}


int main(int argc, char *argv[]) 
{
    ulong arrayCount = atoi(argv[1]);
    ulong arraySize = atoi(argv[2]);
    ulong total = arraySize * arrayCount;
    ulong i;
    
    int *data = malloc(sizeof(int) * total);
    srand(time(NULL));
    rand(); rand(); rand();

    for (i = 0; i < total; i++) {
        data[i] = rand() % arraySize;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (i = 0; i < arrayCount; i++) 
    {
        int *_data = (data + (i * arraySize));
        // display(_data, arraySize, "BEFORE");
        quick_sort( _data, arraySize);
        // if (! check(_data, arraySize)) {
        //     display(_data, arraySize, "ERROR: ");
        // }
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double diffSec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;

    printf("Array count: %d \n", (int)arrayCount);
    printf("Array size: %d \n", (int)arraySize);
    printf("C:                      %f s\n", diffSec);

    return 0;
}
