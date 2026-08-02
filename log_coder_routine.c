#include "codexion.h"


void    log_fct(simulation* monitor, coder* coder_did, int n)
{
    long long          timestamp;

    timestamp = get_current_time() - monitor->start_time;
    
    pthread_mutex_lock(&monitor->log_mutex);
    if (n == 1)
        printf("%lld %d has taken a dongle\n", timestamp, coder_did->id);
    else if (n == 2)
        printf("%lld %d is compiling\n", timestamp, coder_did->id);
    else if (n == 3)
        printf("%lld %d is debugging\n", timestamp, coder_did->id);
    else if (n == 4)
        printf("%lld %d is refactoring\n", timestamp, coder_did->id);
    else if (n == 5)
        printf("%lld %d burned out\n", timestamp, coder_did->id);
    pthread_mutex_unlock(&monitor->log_mutex);
}
void    coder_is_compiling(coder* user,simulation* simulater)
{
    if (check_stop_flag(simulater))
        return;
    user->last_compile_time = get_current_time();
    log_fct(simulater, user, 2);
    usleep(simulater->parsed->time_to_compile * 1000);
}
void    coder_debbuging(simulation* simulater, coder* user)
{
    if (check_stop_flag(simulater))
        return ;
    log_fct(simulater, user, 3);
    usleep(simulater->parsed->time_to_debug * 1000);
}
void    coder_refactoring(simulation* simulater, coder* user)
{
    if (check_stop_flag(simulater))
        return;
    log_fct(simulater, user, 4);
    usleep(simulater->parsed->time_to_refactor * 1000);
}
int check_coder(simulation* simulater)
{
    int     i;
    int     counter;

    i = 0;
    counter = 0;
    while (i < simulater->parsed->number_of_coders)
    {
        if (simulater->all_coders[i].counter_compiling >= simulater->parsed->number_of_compiles_required)
            counter++;
        if (counter == simulater->parsed->number_of_coders)
            return (1);
        i++;
    }
    return (0);
}

void*    coder_routine(void* arg)
{
    simulation*     simulater;
    coder*          user;
    user = (coder*)arg;
    simulater = user->manager;
    while (user->counter_compiling < simulater->parsed->number_of_compiles_required)
    {
        if (user->id % 2 == 0)
        {
            if(dongles_requeste(user, user->right_dongle, simulater) == NULL)
                return (NULL);
            if(dongles_requeste(user, user->left_dongle, simulater) == NULL)
                return (NULL);
            if(check_stop_flag(simulater))
                return (NULL);
        }
        else
        {
            if(dongles_requeste(user, user->left_dongle, simulater) == NULL)
                return (NULL);
            if(dongles_requeste(user, user->right_dongle, simulater) == NULL)
                return (NULL);
            if(check_stop_flag(simulater))
                return (NULL);
        }
        coder_is_compiling(user, simulater);
        user->counter_compiling++;
        put_dongel(user->right_dongle, simulater);
        put_dongel(user->left_dongle, simulater);
        coder_debbuging(simulater, user);
        coder_refactoring(simulater, user);
    }
    return (NULL);
}
void*    burn_out_detecteur(void* arg)
{
    int                 i;
    int                 j;
    simulation*         simulater;

    simulater = (simulation*) arg;
    i = 0;
    j = 0;
    while(1)
    {
        if ((get_current_time() - simulater->all_coders[i].last_compile_time) >= simulater->parsed->time_to_burnout)
        {
            log_fct(simulater, &simulater->all_coders[i], 5);
            pthread_mutex_lock(&simulater->flag_mutex);
            simulater->stop_flag = 1;
            pthread_mutex_unlock(&simulater->flag_mutex);
            while (j < simulater->parsed->number_of_coders)
            {
                pthread_cond_broadcast(&simulater->all_dongles[j].dongle_cond);
                j++;
            }
            return (NULL);
        }
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
        i = (i+1) % (simulater->parsed->number_of_coders);
        usleep(1000);
    }
    return (NULL);
}


int     check_stop_flag(simulation* simulater)
{
    pthread_mutex_lock(&simulater->flag_mutex);
    if(simulater->stop_flag == 1)
        return (pthread_mutex_unlock(&simulater->flag_mutex), 1);
    pthread_mutex_unlock(&simulater->flag_mutex);
    return (0);
}