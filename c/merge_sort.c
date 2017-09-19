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


void merge_sort(int *left, const int size)
{
    if (size <= 1) {
        return;
    }

    const int middleId = (int)(size / 2); // middleId also represent the "new size" of left
    if (middleId > 1) {
        merge_sort(left, middleId);
    }

    int *right = (left + middleId); 
    const int rightSize = size - middleId;
    if (rightSize > 1) {
        merge_sort(right, rightSize);
    }

    // the values in left and right are sorted
    // but value in right may be smaller than some in left

    // as long as values in left are < than the one in right
    // do nothing because there are at the correct place
    
    // as soon as the first value in right is < than the one in left, 
    // add them in order to a temp array
    // until we evaluated all values in left or right

    // add the remaining value from left in the temp array
    // they are sorted already but have to move location inside left
    // remaining values in right are sorted and in the correct place

    // so finally piece things together in order:
    // first values we didn't touched > temp array > remaining values in right

    int *tmpArray; 
    int leftId = 0, rightId = 0, tmpId = 0;
    int firstUnsortedId = -1;

    while (leftId < middleId && rightId < rightSize) 
    {
        if (left[leftId] <= right[rightId]) 
        {
            if (firstUnsortedId > -1) 
            {
                tmpArray[ tmpId++ ] = left[leftId];
            }
            
            leftId++;
        } 
        else
        {
            if (firstUnsortedId == -1) 
            {
                firstUnsortedId = leftId;

                tmpArray = malloc(sizeof(int) * (size - leftId));
            }

            tmpArray[ tmpId++ ] = right[ rightId++ ];
        }
    }

    if (firstUnsortedId > -1) {
        int i;
        // add the remaining left values to the tmpArray
        for (i = leftId; i < middleId; i++) {
            tmpArray[ tmpId++ ] = left[i];
        }
        // the remaining values in right are already at the end of left
        // so no need to add them to tmpArray before coping tmpArray in left

        // copy back the tmpArray inside left after the first values that we didn't moved
        int tmpArraySize = (middleId + rightId) - firstUnsortedId;
        for (i = 0; i < tmpArraySize; ++i)
        {
            left[ firstUnsortedId + i ] = tmpArray[i];
        }

        free(tmpArray);
    }
    // else: this means that all values in left were < to the values of right
    // so no need to do anything else since everything is already sorted !
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
    int arrayCount = atoi(argv[1]);
    int arraySize = atoi(argv[2]);
    int total = arraySize * arrayCount;
    
    int *data = malloc(sizeof(int) * total);
    int i;
    srand(time(NULL));
    rand(); rand(); rand();

    for (i = 0; i < total; i++) { 
        data[i] = rand() % arraySize;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (i = 0; i < arrayCount; i++) {
        int *a = (data + (i * arraySize));
        // display(a, arraySize, "AVANT: ");
        merge_sort(a, arraySize);
        // if (! check(a, arraySize)) {
        //     display(a, arraySize, "ERROR: ");
        // }
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double diffSec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;

    printf("Array count: %d \n", arrayCount);
    printf("Array size: %d \n", arraySize);
    printf("C:                      %f s\n", diffSec);

    return 0;
}

