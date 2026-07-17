#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>


typedef struct dongles dongle;
typedef struct coders coder;
typedef struct simulation simulation;

typedef struct parsing
{
    int             number_of_coders;
    long long       time_to_burnout;
    long long       time_to_compile;
    long long       time_to_debug;
    long long       time_to_refactor;
    long long       number_of_compiles_required;
    long long       dongle_cooldown;
    char*           scheduler;
} parse;

typedef struct coders
{
    int                 id;
    int                 i_compiling;
    long long           last_compile_time;
    dongle*             left_dongle;
    dongle*             right_dongle;
    simulation*         manager;
}coder;

typedef struct t_waiting_slot
{
    coder*              waiting_coder;
    long long           coder_timestamps;
}t_slot;

typedef struct dongles
{
    int                 available;
    long long           release_time;
    pthread_mutex_t     dongle_mutex;
    pthread_cond_t      dongle_cond;
    t_slot               waiting_queue[2];
}dongle;

typedef struct simulation
{
    coder*              all_coders;
    dongle*             all_dongels;
    parse*              parsed;
    pthread_mutex_t     flag_mutex;
    int                 stop_flag;
    long long           clock;
    pthread_mutex_t     log_mutex;
}simulation;
