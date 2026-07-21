#include "codexion.h"

void    show_things(simulation* simu)
{
    coder*      coders;

    coders = simu->all_coders;
    for(int i = 0; i < simu->parsed->number_of_coders; i++)
    {
        printf("the coder %d has in his left dongle %d and in his right dongle %d.\n",coders[i].id, coders[i].left_dongle->id, coders[i].right_dongle->id);
    }
}





int main(int argc, char** argv)
{
    parse*          arg;
    simulation*     simulater;


    arg = ft_parse(argc, argv);
    simulater = inisialize_simulater(arg);
    if(!simulater)
        return(error_join("missing sumilation"),0);
    show_things(simulater);
    return 0;
}
