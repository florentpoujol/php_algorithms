#include <stdlib.h>
#include <time.h>
#include <string.h>

#define REGISTER_TIME(varName, s) \
    printf("Register time %s \n", s); \
    struct timespec varName; \
    clock_gettime(CLOCK_MONOTONIC_RAW, &varName);



void array_print_inline(int array[], int size, char *text)
{
    int i;
    printf("%s size=%d : ", text, size);
    for (i = 0; i < size; i++) {

        printf("%d ", array[i]);
    }
    printf("\n");
}


void array_print(int array[], int size, char *text)
{
    int i;
    printf("%s size=%d\n", text, size);
    for (i = 0; i < size; i++) {
        printf("%d : %d\n", i, array[i]);
    }
    printf("\n");
}


// https://stackoverflow.com/questions/6127503/shuffle-array-in-c
void array_shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}


int array_sorted(int *array, int size)
{
    for (int i=1; i < size; i++) { 
        if (array[i-1] > array[i]) {
            return 0;
        }
    }
    return 1;
}


void str_repeat(char *strDest, char *strSource, int count)
{
    strcpy(strDest, "");
    for (int i = 0; i < count; i++) {
        strcat(strDest, strSource);
    }
}


double getDiff(const struct timespec start, const struct timespec end)
{
    return (double) (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
}
