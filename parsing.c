#include "codexion.h"

int	ft_isdigit(char* c)
{
    int i;

    i = 0;
    while (c[i]!= '\0')
    {
        if (!(c[i] >= 48 && c[i] <= 57))
		    return (0);
        i++;
    }
    return 1;
}


int ft_check_arg(char**argv, int argc)
{
    int i;
    if (argc != 9)
        return 0;
    if(atoi(argv[1]) <= 0)
        return 0;
    i = 2;
    while (i <= 7)
    {
        if(!ft_isdigit(argv[i]))
            return 0;
        if (atoi(argv[i]) <= 0)
            return 0;
        i++;
    }
    if (strcmp(argv[8], "edf") != 0 && strcmp(argv[8], "fifo") != 0)
        return 0;
    return 1;
}
parse* ft_parse(int argc,char**argv)
{
    parse   *arg;

    if(!ft_check_arg(argv,argc))
    {
        printf("loooool your arg are false ");
        return NULL;
    }
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

int main(int argc, char** argv)
{
    parse* arg;

    arg = ft_parse(argc, argv);


    printf("%d", ft_isdigit(argv[1]));

    if(arg)
    { 
        printf("number of coders is: %d\n",arg->number_of_coders );
        printf("time_to_burnout : %ld\n",arg -> time_to_burnout );
        printf("time_to_compile : %ld\n",arg -> time_to_compile );
        printf("time_to_debug : %ld\n",arg -> time_to_debug );
        printf("time_to_refactor : %ld\n",arg -> time_to_refactor );
        printf("number_of_compiles_required : %ld\n",arg -> number_of_compiles_required );
        printf("dongle_cooldown : %ld\n",arg -> dongle_cooldown );
        printf("scheduler : %s\n",arg -> scheduler );
    }
    return 0;
}