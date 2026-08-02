#include "codexion.h"

int   start_simulation(int argc, char** argv)
{
    parse*          arg;
    simulation*     simulater;
    int             i;

    i = 0;

    arg = ft_parse(argc, argv);
    simulater = inisialize_simulater(arg);
    if(!simulater)
        return(error_join("missing sumilation"),0);
    while (i < arg->number_of_coders)
    {
        if ((pthread_create(&(simulater->all_coders[i].coder_thread), NULL, coder_routine, &(simulater->all_coders[i]))) != 0)
            return (error_join("errore while creating coders."), 0);
        i++;
    }
    if ((pthread_create(&(simulater->monitor), NULL, burn_out_detecteur, simulater)) != 0)
            return (error_join("errore while creating simuleter thread."), 0);
    i = 0;
    while (i < arg->number_of_coders)
    {
        if (pthread_join(simulater->all_coders[i].coder_thread, NULL) != 0)
            return (error_join("errore while joining coders."), 0);
        i++;
    }
    if (pthread_join(simulater->monitor, NULL) != 0)
        return (error_join("errore while joining simulleter thread."), 0);
    ft_distroy(simulater);
    return (1);
}

int main(int argc, char** argv)
{
    start_simulation(argc, argv);
    return 0;
}
