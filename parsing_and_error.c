#include "codexion.h"

void    error_join(char* err)
{
    fprintf(stderr, "UNFortunately there is an error: %s", err);
}
int	ft_isdigit(char* c)
{
    int i;

    i = 0;
    while (c[i]!= '\0')
    {
        if (!(c[i] >= 48 && c[i] <= 57) && c[i] != '+')
		    return (0);
        i++;
    }
    return 1;
}

int ft_check_arg(char**argv, int argc)
{
    int i;
    if (argc != 9)
		return (error_join("the number of arg most be 6 number and one scheduler"), 0);
    i = 1;
    while (i <= 7)
    {
        if(!ft_isdigit(argv[i]))
            return (error_join("arguments most be an valid positive int"), 0);
        if (atoi(argv[i]) <= 0 && i != 7)
            return (error_join("arguments most be an valid positive int"), 0);
        i++;
    }
    if (strcmp(argv[8], "edf") != 0 && strcmp(argv[8], "fifo") != 0)
        return (error_join("the scheduler arg most be either an 'fifo' or 'edf'"), 0);
    return 1;
}

parse* ft_parse(int argc,char**argv)
{
    parse   *arg;

    if(!ft_check_arg(argv,argc))
        return NULL;
    arg = (parse*)malloc(sizeof(parse));
    if (!arg)
        return NULL;
    arg -> number_of_coders = atoi(argv[1]);   
    arg -> time_to_burnout = atoi(argv[2]);   
    arg -> time_to_compile = atoi(argv[3]);   
    arg -> time_to_debug = atoi(argv[4]);   
    arg -> time_to_refactor = atoi(argv[5]);   
    arg -> number_of_compiles_required = atoi(argv[6]);   
    arg -> dongle_cooldown = atoi(argv[7]);
    arg -> scheduler = argv[8];
    return arg;
}
