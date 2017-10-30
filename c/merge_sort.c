#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utils.c"


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
    register int leftId = 0, rightId = 0, tmpId = 0;
    int firstUnsortedId = -1;

    while (leftId < middleId && rightId < rightSize) 
    {
        if (left[leftId] <= right[rightId]) 
        {
            if (firstUnsortedId > -1) 
            {
                tmpArray[ tmpId++ ] = left[leftId];
            }
            
            ++leftId;
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
        // add the remaining left values to the tmpArray
        while (leftId < middleId) {
            tmpArray[ tmpId++ ] = left[ leftId++ ];
        }
        // the remaining values in right are already at the end of left
        // so no need to add them to tmpArray before coping tmpArray in left

        // copy back the tmpArray inside left after the first values that we didn't moved
        for (register int i = 0; i < tmpId; ++i) // tmpId is the size of tmpArray
        {
            left[ firstUnsortedId++ ] = tmpArray[ i ];
        }

        free(tmpArray);
    }
    // else: this means that all values in left were < to the values of right
    // so no need to do anything else since everything is already sorted !
}



int main(int argc, char *argv[]) 
{
    int arrayCount = atoi(argv[1]);
    int arraySize = atoi(argv[2]);
    
    int *array = malloc(sizeof(int) * arraySize);
    int i;
    srand(time(NULL)); // needed for array_shuffle()

    for (i = 0; i < arraySize; i++) { 
        array[i] = i;
    }
    array_shuffle(array, arraySize);

    struct timespec shuffle_start, shuffle_end;
    double shuffle_total = 0.0;

    REGISTER_TIME(start, "start");

    for (i = 0; i < arrayCount; i++) {        
        merge_sort(array, arraySize);
        // if (! array_sorted(array, arraySize)) {
        //     printf("ERROR array not sorted \n");
        //     // display(array, arraySize, "ERROR: ");
        // }
        // note: when the 4 lines above is decommented, the mesured time is actually smaller...

        clock_gettime(CLOCK_MONOTONIC_RAW, &shuffle_start);
        array_shuffle(array, arraySize);
        clock_gettime(CLOCK_MONOTONIC_RAW, &shuffle_end);
        shuffle_total += getDiff(shuffle_start, shuffle_end);
    }

    REGISTER_TIME(end, "merge_sort");

    
    double diffSec = getDiff(start, end) - shuffle_total;

    printf("Array count: %d \n", arrayCount);
    printf("Array size: %d \n", arraySize);
    printf("C:                      %f s\n", diffSec);
    printf("C (shuffle time):       %f s\n", shuffle_total);

    return 0;
}
