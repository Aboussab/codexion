#include "codexion.h"

int     check_stop_flag(simulation* simulater)
{
    pthread_mutex_lock(&simulater->flag_mutex);
    if(simulater->stop_flag == 1)
        return (pthread_mutex_unlock(&simulater->flag_mutex), 1);
    pthread_mutex_unlock(&simulater->flag_mutex);
    return (0);
}
int check_coder(simulation* simulater)
{
    int     i;
    int     counter;

    i = 0;
    counter = 0;
    while (i < simulater->parsed->number_of_coders)
    {
        pthread_mutex_lock(&simulater->all_coders[i].counter_compiling_mutex);
        if (simulater->all_coders[i].counter_compiling >= simulater->parsed->number_of_compiles_required)
            counter++;
        if (counter == simulater->parsed->number_of_coders)
            return (pthread_mutex_unlock(&simulater->all_coders[i].counter_compiling_mutex), 1);
        pthread_mutex_unlock(&simulater->all_coders[i].counter_compiling_mutex);
        i++;
    }
    return (0);
}
int     burn_out_condition(simulation* simulater, int i)
{
    long long           time_left;
    int                 n;

    pthread_mutex_lock(&simulater->all_coders[i].counter_compiling_mutex);
    n = simulater->all_coders[i].counter_compiling - simulater->parsed->number_of_compiles_required;
    time_left = (get_current_time() - simulater->all_coders[i].last_compile_time);
    pthread_mutex_unlock(&simulater->all_coders[i].counter_compiling_mutex);
    if (time_left >= simulater->parsed->time_to_burnout && n != 0)
        return (1);
    else
        return (0);
}
void*    detect_burn_out(void* arg)
{
    int                 i;
    int                 j;
    simulation*         simulater;

    simulater = (simulation*) arg;
    i = 0;
    while (1)
    {
        if(check_coder(simulater))
        {
            pthread_mutex_lock(&simulater->flag_mutex);
            simulater->stop_flag = 1;
            pthread_mutex_unlock(&simulater->flag_mutex);
            j = 0;
            while (j < simulater->parsed->number_of_coders)
            {
                pthread_cond_broadcast(&simulater->all_dongles[j].dongle_cond);
                j++;
            }
            return (NULL);
        }
        else if (burn_out_condition(simulater, i))
        {
            pthread_mutex_lock(&simulater->flag_mutex);
            simulater->stop_flag = 1;
            pthread_mutex_unlock(&simulater->flag_mutex);
            log_fct(simulater, simulater->all_coders[i].id, 5);
            j = 0;
            while (j < simulater->parsed->number_of_coders)
            {
                pthread_cond_broadcast(&simulater->all_dongles[j].dongle_cond);
                j++;
            }
            break;
        }
        i = (i+1) % (simulater->parsed->number_of_coders);
        usleep(1000);
    }
    return (NULL);
}
