#include "codexion.h"

void*   run(int argc, char** argv)
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
        pthread_create(simulater->all_coders[i].coder_thread, NULL, coder_routine, &(simulater->all_coders[i]));
    }
    pthread_create(simulater->monitor, NULL, burn_out_detecteur, simulater);
    i = 0;
    while (i < arg->number_of_coders)
    {
        pthread_join(simulater->all_coders[i].coder_thread, NULL);
    }
    pthread_join(simulater->monitor, NULL);

    
}

int main(int argc, char** argv)
{
    parse*          arg;
    simulation*     simulater;

    run(argc, argv);
    return 0;
}
