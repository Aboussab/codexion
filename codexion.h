#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>


typedef struct parsing
{
    int     number_of_coders;
    long    time_to_burnout;
    long    time_to_compile;
    long    time_to_debug;
    long    time_to_refactor;
    long    number_of_compiles_required;
    long    dongle_cooldown;
    char*   scheduler;
} parse;
