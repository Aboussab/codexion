#include "codexion.h"


static void    coder_is_compiling(coders* coder,simulation* simulater)
{
    pthread_mutex_lock(&coder->counter_compiling_mutex);
    coder->last_compile_time = get_current_time();
    pthread_mutex_unlock(&coder->counter_compiling_mutex);
    log_fct(simulater, coder->id, 2);
    usleep(simulater->parsed->time_to_compile * 1000);
}
static void    coder_debbuging(simulation* simulater, coders* coder)
{
    log_fct(simulater, coder->id, 3);
    usleep(simulater->parsed->time_to_debug * 1000);
}
static void    coder_refactoring(simulation* simulater, coders* coder)
{
    log_fct(simulater, coder->id, 4);
    usleep(simulater->parsed->time_to_refactor * 1000);
}
static int         coder_finished_compile(coders* coder, long long number_of_compiles_required)
{
    pthread_mutex_lock(&coder->counter_compiling_mutex);
    if (coder->counter_compiling < number_of_compiles_required)
        return(pthread_mutex_unlock(&coder->counter_compiling_mutex), 1);
    pthread_mutex_unlock(&coder->counter_compiling_mutex);
    return (0);
}
static int take_dongles(coders* coder, simulation* simulater)
{
    if (coder->id % 2 == 0)
    {
        usleep(100);
        if(dongles_requeste(coder, coder->right_dongle, simulater) == 0)
            return (0);
        if(dongles_requeste(coder, coder->left_dongle, simulater) == 0)
            return (0);
    }
    else
    {
        if(dongles_requeste(coder, coder->left_dongle, simulater) == 0)
            return (0);
        if(dongles_requeste(coder, coder->right_dongle, simulater) == 0)
            return (0);
    }
    return (1);
}

void*    coder_routine(void* arg)
{
    simulation*     simulater;
    coders*          coder;

    coder = (coders*)arg;
    simulater = coder->manager;
    pthread_mutex_lock(&simulater->flag_mutex);
    while (simulater->start_flag == 0)
    {
        pthread_cond_wait(&simulater->start_simulation, &simulater->flag_mutex);
    }
    pthread_mutex_unlock(&simulater->flag_mutex);
    while (coder_finished_compile(coder, simulater->parsed->number_of_compiles_required))
    {
        if (take_dongles(coder, simulater) == 0)
            return (NULL);
        coder_is_compiling(coder, simulater);
        put_dongel(coder->right_dongle, simulater);
        put_dongel(coder->left_dongle, simulater);
        coder_debbuging(simulater, coder);
        coder_refactoring(simulater, coder);
        if (check_stop_flag(simulater))
            return (NULL);
        pthread_mutex_lock(&coder->counter_compiling_mutex);
        coder->counter_compiling++;
        pthread_mutex_unlock(&coder->counter_compiling_mutex);
        if (check_stop_flag(simulater))
            return (NULL);
    }
    return (NULL);
}
