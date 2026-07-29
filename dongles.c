#include "codexion.h"


dongle*    dongles_requeste(coder* requester, dongle* orderd, simulation* sim)
{
    struct timespec tsp;

    // release_time for a dongele is the time when it was puted by a coder + time to cooldown
    pthread_mutex_lock(&orderd->dongle_mutex);
    push_queue(orderd, requester, sim);
    while (orderd->waiting_queue[0].waiting_coder != requester || orderd->release_time > get_current_time() || orderd->available == 0)
    {
        tsp.tv_sec = orderd->release_time / 1000;
        tsp.tv_nsec = (orderd->release_time % 1000) * 1000000L;
        pthread_cond_timedwait(&orderd->dongle_cond, &orderd->dongle_mutex, &tsp);
    }
    orderd->available = 0;
    log_fct(sim, requester, 1);
    pop_queue(orderd);
    pthread_mutex_unlock(&orderd->dongle_mutex);

    return (orderd);
}

void    put_dongel(dongle* orderd, simulation* sim)
{
    pthread_mutex_lock(&orderd->dongle_mutex);
    orderd->available = 1;
    orderd->release_time = get_current_time() + sim->parsed->dongle_cooldown;
    pthread_cond_broadcast(&orderd->dongle_cond);
    pthread_mutex_unlock(&orderd->dongle_mutex);
}