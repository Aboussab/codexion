#include "codexion.h"


void    push_queue(dongle* single_dongle, coder* requester, simulation *sim)
{
    int                 index;
    t_slot             tmp;


    index = single_dongle->index;
    single_dongle->waiting_queue[index].waiting_coder = requester;
    if (strcmp(sim->parsed->scheduler, "fifo") == 0)
        single_dongle->waiting_queue[index].coder_timestamps = get_current_time();
    else
        single_dongle->waiting_queue[index].coder_timestamps = requester->last_compile_time + sim->parsed->time_to_burnout;
    if (index == 1)
    {
        if (single_dongle->waiting_queue[index].coder_timestamps < single_dongle->waiting_queue[0].coder_timestamps)
        {
            tmp = single_dongle->waiting_queue[0];
            single_dongle->waiting_queue[0] = single_dongle->waiting_queue[index];
            single_dongle->waiting_queue[index] = tmp;
        }
    }
    single_dongle->index = index + 1;
}

coder*    pop_queue(dongle* single_dongle)
{
    int                 index;
    coder*              requester;

    index = 0;
    if (single_dongle->index == 0)
        return (NULL);
    requester = single_dongle->waiting_queue[index].waiting_coder;
    if (single_dongle->index == 2)
    {
        single_dongle->waiting_queue[0] = single_dongle->waiting_queue[1];
        single_dongle->waiting_queue[1].coder_timestamps = 0;
        single_dongle->waiting_queue[1].waiting_coder = NULL;
    }
    else if (single_dongle->index == 1)
    {
        single_dongle->waiting_queue[0].coder_timestamps = 0;
        single_dongle->waiting_queue[0].waiting_coder = NULL;
    }
    single_dongle->index--;
    return (requester);
}
