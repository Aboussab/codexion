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
            dongles_requeste(user, user->right_dongle, simulater);
            dongles_requeste(user, user->left_dongle, simulater);
        }
        else
        {
            dongles_requeste(user, user->left_dongle, simulater);
            dongles_requeste(user, user->right_dongle, simulater);
        }
        coder_is_compiling(simulater, user);
        put_dongel(user->right_dongle, simulater);
        put_dongel(user->left_dongle, simulater);
        coder_debbuging(simulater, user);
        coder_refactoring(simulater, user);
    }
    return (NULL);
}



void    coder_is_compiling(coder* user,simulation* simulater)
{
    user->last_compile_time = get_current_time();
    log_fct(simulater, user, 2);
    usleep(simulater->parsed->time_to_compile);
    user->counter_compiling++;
}
void    coder_debbuging(simulation* simulater, coder* user)
{
    log_fct(simulater, user, 3);
    usleep(simulater->parsed->time_to_debug);
    // u need to add some way to check every time if a coder burn out or the simulation should stop
}
void    coder_refactoring(simulation* simulater, coder* user)
{
    log_fct(simulater, user, 4);
    usleep(simulater->parsed->time_to_refactor);
}