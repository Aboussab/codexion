/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inisialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboussab <aboussab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:30:20 by aboussab          #+#    #+#             */
/*   Updated: 2026/08/09 19:17:32 by aboussab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_current_time(void)
{
	struct timespec		tv;

	clock_gettime(CLOCK_REALTIME, &tv);
	return (tv.tv_sec * 1000L + tv.tv_nsec / 1000000L);
}

static t_simulation	*allocation_de_object(t_parse *arg)
{
	t_simulation	*simulater;
	t_coders		*coders_array;
	t_dongles		*dongle_array;

	simulater = (t_simulation *) malloc(sizeof(t_simulation));
	if (!simulater)
		return (error_join("the creation of simulater failed."), NULL);
	coders_array = (t_coders *) \
malloc(sizeof(t_coders) * (arg->number_of_coders));
	if (!coders_array)
		return (free(simulater), \
error_join("the creation of coders_array failed."), NULL);
	dongle_array = \
(t_dongles *) malloc(sizeof(t_dongles) * (arg->number_of_coders));
	if (!dongle_array)
		return (free(coders_array), \
free(simulater), error_join("the creation of dongle_array failed."), NULL);
	simulater->all_coders = coders_array;
	simulater->all_dongles = dongle_array;
	simulater->parsed = arg;
	return (simulater);
}

static void	creat_coders(t_simulation *simulater)
{
	int		i;
	int		n;

	n = simulater->parsed->number_of_coders;
	i = 0;
	while (i < n)
	{
		simulater->all_coders[i].id = i + 1;
		simulater->all_coders[i].counter_compiling = 0;
		simulater->all_coders[i].last_compile_time = simulater->start_time;
		simulater->all_coders[i].manager = simulater;
		simulater->all_coders[i].left_dongle = &(simulater->all_dongles[i]);
		pthread_mutex_init(&simulater->all_coders[i].counter_compiling_mutex, \
NULL);
		simulater->all_coders[i].right_dongle = \
&(simulater->all_dongles[(i + 1) % n]);
		i++;
	}
}

static void	creat_dongels(t_simulation *simulater)
{
	int	i;

	i = 1;
	while (i <= simulater->parsed->number_of_coders)
	{
		simulater->all_dongles[i - 1].available = 1;
		simulater->all_dongles[i - 1].id = i;
		simulater->all_dongles[i - 1].release_time = 0;
		simulater->all_dongles[i - 1].size = 0;
		if (pthread_mutex_init\
(&(simulater->all_dongles[i - 1].dongle_mutex), NULL) != 0)
			return (error_join("creation dongel mutex faild"));
		if (pthread_cond_init\
(&(simulater->all_dongles[i - 1].dongle_cond), NULL) != 0)
			return (error_join("creation condetions var faild"));
		simulater->all_dongles[i - 1].waiting_queue[0].waiting_coder = NULL;
		simulater->all_dongles[i - 1].waiting_queue[0].coder_timestamps = 0;
		simulater->all_dongles[i - 1].waiting_queue[1].waiting_coder = NULL;
		simulater->all_dongles[i - 1].waiting_queue[1].coder_timestamps = 0;
		i++;
	}
}

t_simulation	*inisialize_simulater(t_parse *arg)
{
	t_simulation	*simulater;

	simulater = allocation_de_object(arg);
	if (simulater)
	{
		pthread_mutex_init(&simulater->flag_mutex, NULL);
		simulater->stop_flag = 0;
		simulater->start_flag = 0;
		pthread_cond_init(&simulater->start_simulation, NULL);
		simulater->start_time = get_current_time();
		pthread_mutex_init(&simulater->log_mutex, NULL);
		simulater->monitor = 0;
		creat_coders(simulater);
		creat_dongels(simulater);
		return (simulater);
	}
	return (NULL);
}
