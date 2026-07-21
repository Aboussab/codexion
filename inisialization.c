#include "codexion.h"
//ALLOCATION D'ESPACE POUR LE  simulation STRUCT 
simulation*    creation_de_object(parse* arg)
{
    simulation* simulater;
    coder*      coders_array;
    dongle*     dongle_array;

    simulater = (simulation *) malloc(sizeof(simulation));
    if (!simulater)
        return (error_join("the creation of simulater failed."), NULL);
    coders_array = (coder*) malloc(sizeof(coder) * (arg->number_of_coders));
    if (!coders_array)
        return (free(simulater), error_join("the creation of coders_array failed."), NULL);
    dongle_array = (dongle*) malloc(sizeof(dongle) * (arg->number_of_coders));
    if (!dongle_array)
        return (free(coders_array), free(simulater), error_join("the creation of dongle_array failed."), NULL);
    simulater->all_coders = coders_array;
    simulater->all_dongles = dongle_array;
    simulater->parsed = arg;
    return (simulater);
}
//
simulation*    inisialize_simulater(parse* arg)
{
    struct timeval tv;
    simulation* simulater;

    simulater = creation_de_object(arg);
    if (simulater)
    {
        gettimeofday(&tv, NULL);
        pthread_mutex_init(&simulater->flag_mutex, NULL);
        simulater->stop_flag = 0;
        simulater->start_time = tv.tv_sec * 1000L + tv.tv_usec / 1000;
        pthread_mutex_init(&simulater->log_mutex, NULL);
        simulater->monitor = 0;
        return (simulater);
    }
    return(NULL);
}
// here we have a fct that take a simululater and inisalize the coder array ;

void creat_coders(simulation* simulater)
{
    int     i;
    int     n;

    n = simulater->parsed->number_of_coders;
    i = 1;
    while (i <= n)
    {
        simulater->all_coders[i-1].id = i;
        simulater->all_coders[i-1].counter_compiling = 0;
        simulater->all_coders[i-1].last_compile_time = simulater->start_time;
        simulater->all_coders[i-1].manager = simulater;
        simulater->all_coders[i-1].left_dongle = &(simulater->all_dongles[(i + 1) % n]);
        simulater->all_coders[i-1].right_dongle = &(simulater->all_dongles[i % n]);
        i++;
    }
    
}
 void   creat_dongels(simulation* simulater)
 {
    int     i;

    i = 1;
    while (i <= simulater->parsed->number_of_coders)
    {
        simulater->all_dongles[i-1].available = 1;
        simulater->all_dongles[i-1].release_time = 0;
        if (pthread_mutex_init(&(simulater->all_dongles[i-1].dongle_mutex), NULL) != 0)
            return(error_join("creation dongel mutex faild"), 0);
        if (pthread_cond_init(&(simulater->all_dongles[i-1].dongle_cond), NULL) != 0)
            return(error_join("creation condetions var faild"), 0);
        simulater->all_dongles[i-1].waiting_queue[0].waiting_coder = NULL;
        simulater->all_dongles[i-1].waiting_queue[0].coder_timestamps = 0;
        simulater->all_dongles[i-1].waiting_queue[1].waiting_coder = NULL;
        simulater->all_dongles[i-1].waiting_queue[1].coder_timestamps = 0;
        i++;
    }
}
