#include "codexion.h"


void    ft_distroy(simulation*  simulater)
{
    int         i;

    i = 0;
    while(i <= simulater->parsed->number_of_coders - 1)
    {
        pthread_mutex_destroy(&(simulater->all_dongles[i].dongle_mutex));
        pthread_cond_destroy(&(simulater->all_dongles[i].dongle_cond));
        i++;
    }
    pthread_mutex_destroy(&(simulater->flag_mutex));
    pthread_mutex_destroy(&(simulater->log_mutex));
    free(simulater->all_dongles);
    free(simulater->all_coders);
    free(simulater->parsed);
    free(simulater);
}

// /./codexion 15 120 60 60 60 5 60 fifo 