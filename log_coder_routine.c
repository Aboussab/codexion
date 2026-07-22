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

void    coder_routine(simulation* simulater, coder* user)
{
    while(!(simulater->stop_flag))
    {
        if(user->left_dongle->available)
        {
            user->left_dongle->available = 0;
            pthread_mutex_lock(&(user->left_dongle->dongle_mutex));
            log_fct(simulater, user, 1);
            usleep(simulater->parsed->time_to_compile);
            pthread_mutex_unlock(&(user->left_dongle->dongle_mutex));
            user->left_dongle->available = 1;
        }
        else
        {
            usleep(simulater->parsed->time_to_compile);
            continue;
        }
        if(user->left_dongle->available)
        {
            user->left_dongle->available = 0;
            pthread_mutex_lock(&(user->right_dongle->dongle_mutex));
            log_fct(simulater, user, 1);
            usleep(simulater->parsed->time_to_compile);
            pthread_mutex_unlock(&(user->right_dongle->dongle_mutex));
            
            usleep(simulater->parsed->time_to_debug);
            usleep(simulater->parsed->time_to_compile);
            user->left_dongle->available = 1; 
        }
        else
        {
            usleep(simulater->parsed->time_to_compile);
            continue;
        }
    }
}
