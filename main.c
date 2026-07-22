#include "codexion.h"

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
