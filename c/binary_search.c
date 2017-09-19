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

    return binary_search(pmiddle, size - middleId, target);    
}


int main(int argc, char *argv[]) 
{
    int arrayCount = atoi(argv[1]);
    int arraySize = atoi(argv[2]);
    int total = arraySize * arrayCount;
    
    int *data = malloc(sizeof(int) * total);
    int i, j = 0;
    srand(time(NULL));
    rand(); rand(); rand();

    for (i = 0; i < total; i++) {
        if (i % arraySize == 0) {
            j = 0;
        }

        data[i] = j;
        j++;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for (i = 0; i < arrayCount; i++) {
        int target = rand() % arraySize;
        if (! binary_search( (data + (i * arraySize)), arraySize, target)) {
            printf("wrong target=%d \n", target);
            display( (data + (i * arraySize)), arraySize, "");
        }
        // binary_search((data + i), arraySize, targets[i]);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double diffSec = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;

    printf("Array count: %d \n", arrayCount);
    printf("Array size: %d \n", arraySize);
    printf("C:                      %f s\n", diffSec);

    return 0;
}

