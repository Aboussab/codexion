#include "codexion.h"

int main(int argc, char** argv)
{
    pthread_t       monitor;
    parse*          arg;
    pthread_mutex_t flag_mutex;
    pthread_mutex_t log_mutex;
    simulation*     simulater;
    coder*         coders_array;
    dongle*         dongle_array;

    arg = ft_parse(argc, argv);

    if (arg)
    {
        creation_de_object(simulater, coders_array, dongle_array, arg);
    }
    return 0;
}

// int main(int argc, char** argv)
// {
//     parse*          arg;

//     arg = ft_parse(argc, argv);

//     if (arg)
//     {
//         printf("VALID OUTPUT\n");
//         }
//     return 0;
// }