#include "codexion.h"

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

void    inisialize_simulater(simulation * simulater, pthread_mutex_t flag_mutex, pthread_mutex_t log_mutex, pthread_t monitor)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    simulater->flag_mutex = flag_mutex;
    simulater->stop_flag = 0;
    simulater->start_time = tv.tv_sec * 1000L + tv.tv_usec / 1000;
    simulater->log_mutex = log_mutex;
    simulater->monitor = monitor;
}