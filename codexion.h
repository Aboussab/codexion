#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>


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

typedef struct coders coders;
typedef struct simulation simulation;


typedef struct t_waiting_slot
{
    coders*              waiting_coder;
    long long           coder_timestamps;
}t_slot;
// this is the key of the priority value for each scheduler methode if it was an:
        // -FIFO we automaticly holds the time that threads requeste the dongle 
        // -the EDF we hold the store the burnout deadline which is last_compile_time + time_to_burnout.
    //and for both of theme we always take the smallest values between the tow coder.

typedef struct dongles
{
    int                 id;
    int                 available;
    long long           release_time;
    pthread_mutex_t     dongle_mutex;
    pthread_cond_t      dongle_cond;
    t_slot              waiting_queue[2];
    int                 size;
}dongles;

typedef struct coders
{
    int                 id;
    int                 counter_compiling;
    long long           last_compile_time;
    dongles*             left_dongle;
    dongles*             right_dongle;
    pthread_mutex_t        counter_compiling_mutex;
    pthread_t           coder_thread;
    simulation*         manager;
}coders;

typedef struct simulation
{
    coders*              all_coders;
    dongles*             all_dongles;
    parse*               parsed;
    pthread_mutex_t      flag_mutex;
    pthread_cond_t       start_simulation;
    int                  stop_flag;              // this flags is for the stop flags either all coders finsish or one of theme burne out 
    int                  start_flag;              // this flags is for the stop flags either all coders finsish or one of theme burne out 
    long long            start_time;                  // this one is for giving the time  to log 
    pthread_mutex_t      log_mutex;
    pthread_t            monitor;
}simulation;


void                        error_join(char* err);
parse*                      ft_parse(int argc,char**argv);
long                        get_current_time();
void                        log_fct(simulation* monitor, int i, int n);
simulation*                 inisialize_simulater(parse* arg);
int                         start_simulation(int argc, char** argv);
void*                       coder_routine(void* arg);
int                         dongles_requeste(coders* coder, dongles* dongle, simulation* sim);
void                        put_dongel(dongles* dongle, simulation* sim);
int                         check_stop_flag(simulation* simulater);
void*                       detect_burn_out(void* arg);
void                        ft_distroy(simulation*  simulater);

