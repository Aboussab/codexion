#include "codexion.h"

static void    push_queue(dongles* dongle, coders* coder, simulation *sim)
{
    int                 index;
    t_slot              tmp;

    index = dongle->size;
    if(index >= 2)
        return;
    dongle->waiting_queue[index].waiting_coder = coder;
    if (strcmp(sim->parsed->scheduler, "fifo") == 0)
        dongle->waiting_queue[index].coder_timestamps = get_current_time();
    else
        dongle->waiting_queue[index].coder_timestamps = coder->last_compile_time + sim->parsed->time_to_burnout;
    if (index == 1)
    {
        if (dongle->waiting_queue[index].coder_timestamps < dongle->waiting_queue[0].coder_timestamps)
        {
            tmp = dongle->waiting_queue[0];
            dongle->waiting_queue[0] = dongle->waiting_queue[index];
            dongle->waiting_queue[index] = tmp;
        }
    }
    dongle->size = index + 1;
}
static coders*    pop_queue(dongles* dongle)
{
    int                 index;
    coders*              coder;

    index = 0;
    if (dongle->size == 0)
        return (NULL);
    coder = dongle->waiting_queue[index].waiting_coder;
    if (dongle->size == 2)
    {
        dongle->waiting_queue[0] = dongle->waiting_queue[1];
        dongle->waiting_queue[1].coder_timestamps = 0;
        dongle->waiting_queue[1].waiting_coder = NULL;
    }
    else if (dongle->size == 1)
    {
        dongle->waiting_queue[0].coder_timestamps = 0;
        dongle->waiting_queue[0].waiting_coder = NULL;
    }
    dongle->size--;
    return (coder);
}
int    dongles_requeste(coders* coder, dongles* dongle, simulation* sim)
{
    struct timespec tsp;

    pthread_mutex_lock(&dongle->dongle_mutex);
    push_queue(dongle, coder, sim); // pushing the requster into the heap_queue
    // then check if the simulation didn't end yet and if the requster is the top also the dongle finished the cooldown time and also available
    while (dongle->waiting_queue[0].waiting_coder != coder || dongle->release_time > get_current_time() || dongle->available == 0)
    {
        if (check_stop_flag(sim))
        return (pthread_mutex_unlock(&dongle->dongle_mutex), 0);
        tsp.tv_sec = dongle->release_time / 1000;
        tsp.tv_nsec = (dongle->release_time % 1000) * 1000000L;
        pthread_cond_timedwait(&dongle->dongle_cond, &dongle->dongle_mutex, &tsp);
    }
    pthread_mutex_unlock(&dongle->dongle_mutex);
    if (check_stop_flag(sim))
        return (0);
    else
    {
        pthread_mutex_lock(&dongle->dongle_mutex);
        dongle->available = 0;
        log_fct(sim, coder->id, 1);
        pop_queue(dongle);
        pthread_mutex_unlock(&dongle->dongle_mutex);
    }
    return (1);
}
void    put_dongel(dongles* dongle, simulation* sim)
{
    pthread_mutex_lock(&dongle->dongle_mutex);
    dongle->available = 1;
    dongle->release_time = get_current_time() + sim->parsed->dongle_cooldown;
    pthread_cond_broadcast(&dongle->dongle_cond);
    pthread_mutex_unlock(&dongle->dongle_mutex);
}
