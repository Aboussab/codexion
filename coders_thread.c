/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboussab <aboussab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:43:58 by aboussab          #+#    #+#             */
/*   Updated: 2026/08/09 19:17:58 by aboussab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coder_finished_compile(t_coders *coder, \
long long number_of_compiles)
{
	pthread_mutex_lock(&coder->counter_compiling_mutex);
	if (coder->counter_compiling < number_of_compiles)
		return (pthread_mutex_unlock(&coder->counter_compiling_mutex), 1);
	pthread_mutex_unlock(&coder->counter_compiling_mutex);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_simulation		*simulater;
	t_coders			*coder;

	coder = (t_coders *)arg;
	simulater = coder->manager;
	pthread_mutex_lock(&simulater->flag_mutex);
	while (simulater->start_flag == 0)
		pthread_cond_wait(&simulater->start_simulation, &simulater->flag_mutex);
	pthread_mutex_unlock(&simulater->flag_mutex);
	while (\
coder_finished_compile(coder, simulater->parsed->n_compiles_required))
	{
		if (take_dongles(coder, simulater) == 0)
			return (NULL);
		coder_doing(coder, simulater);
		if (check_stop_flag(simulater))
			return (NULL);
		pthread_mutex_lock(&coder->counter_compiling_mutex);
		coder->counter_compiling++;
		pthread_mutex_unlock(&coder->counter_compiling_mutex);
		if (check_stop_flag(simulater))
			return (NULL);
	}
	return (NULL);
}
