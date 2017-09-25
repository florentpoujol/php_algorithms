
#define REGISTER_TIME(varName) struct timespec varName; \
    clock_gettime(CLOCK_MONOTONIC_RAW, &varName);


void array_print(int array[], int size, char *text)
{
    int i;
    printf("%s size=%d : ", text, size);
    for (i = 0; i < size; i++) {

        printf("%d ", array[i]);
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