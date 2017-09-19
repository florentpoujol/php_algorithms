
/*

Actually this doesn't work 
soimetimes it works 
but sometimes some values gets changed in memory, I don't know why
also when the numbers become too big, I get different kind of memory errors
*/

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


void quick_sort(int *array, const size_t size)
{
    if (size <= 1) {
        return;
    }

    int d = 1; // debug
    if (d) {
        printf("------------------------ \n");
        display(array, size, "");      
        printf("array[6]=%d \n", array[6]);
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

    int halfSize = (int)(size/2);
    int pivot = array[halfSize];
    if (pivot < first) {
        pivot = first;
    } else if (pivot > last) {
        pivot = last;
    }

    if (d) {
        printf("first=%d pivot=%d last=%d\n", first, pivot, last);      
    }
    
    // moves values
    printf("array[6]=%d %d p=%p \n", array[6], *(array + 6), (array + 6));
    int *left  = calloc(halfSize, sizeof(int));
    int *right = calloc(halfSize, sizeof(int));
    printf("array[6]=%d %d p=%p \n", array[6], *(array + 6), (array + 6));
    int value;
    size_t i, leftSize = 0, middleSize = 0, rightSize = 0;

    for (i = 0; i < size; i++) {
        value = array[i];
        if (value == 33) {
            printf("************** \n");
            printf("value=33 i=%d rightSize=%d array[i]=%d \n", (int)i, (int)rightSize, array[i]);
            printf("************** \n");
        }
        if (value < pivot) {
            left[leftSize] = value;
            leftSize++;
        } else if (value > pivot) {
            right[rightSize] = value;
            rightSize++;
        } else {
            middleSize++;
        }        
    }

    if (d) {
        printf("ls=%d ms=%d rs=%d \n", (int)leftSize, (int)middleSize, (int)rightSize);      
    }

    display(left, leftSize, "left before:");
    display(right, rightSize, "right before:");
    if (leftSize > 1) {
        quick_sort(left, leftSize);
        display(left, leftSize, "left after:");
    }

    if (rightSize > 1) {
        quick_sort(right, rightSize);
        display(right, rightSize, "right after:");
    }

    // merge
    for (i = 0; i < leftSize; i++) {
        array[i] = left[i];
    }
    for (i = 0; i < middleSize; i++) {
        array[ (leftSize + i) ] = pivot;
    }
    int offset = leftSize + middleSize;
    for (i = 0; i < rightSize; i++) {
        array[ (offset + i) ] = right[i];
    }

    // free(left);
    // free(right);

    display(array, size, "result:");
    
}




int main(int argc, char *argv[]) 
{
    ulong arrayCount = atoi(argv[1]);
    ulong arraySize = atoi(argv[2]);
    ulong total = arraySize * arrayCount;
    ulong i;
    
    // int *data = malloc(sizeof(int) * total);
    int data[] = {1, 5, 9, 5, 4, 17, 3, 15, 2, 7, 9, 19, 13, 10, 9, 8, 5, 8, 6, 10};
    srand(time(NULL));
    rand(); rand(); rand();

    for (i = 0; i < total; i++) {
        // data[i] = rand() % arraySize;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (i = 0; i < arrayCount; i += arraySize) {
        display(data + i, arraySize, "BEFORE");
        quick_sort( (data + (i * arraySize)), arraySize);
        display(data + i, arraySize, "AFTER");
    }
    // free(data);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double diffSec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;

    printf("Array count: %d \n", (int)arrayCount);
    printf("Array size: %d \n", (int)arraySize);
    printf("C:                      %f s\n", diffSec);

    return 0;
}
