#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>


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
    int                 counter_compiling;
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
    dongle*             all_dongles;
    parse*              parsed;
    pthread_mutex_t     flag_mutex;
    int                 stop_flag;              // this flags is for the stop flags either all coders finsish or one of theme burne out 
    long long           start_time;                  // this one is for giving the time  to log 
    pthread_mutex_t     log_mutex;
    pthread_t           monitor;
}simulation;

parse* ft_parse(int argc,char**argv);
simulation*    creation_de_object(parse* arg);
simulation*    inisialize_simulater(parse* arg);
void creat_coders(simulation* simulater);
void   creat_dongels(simulation* simulater);
void    error_join(char* err);
// void    creation_de_object(simulation* simulater, coder* coders_array, dongle* dongle_array, parse* arg);