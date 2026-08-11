/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboussab <aboussab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:19:02 by aboussab          #+#    #+#             */
/*   Updated: 2026/08/09 19:17:16 by aboussab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	push_queue(t_dongles *dongle, t_coders *coder, t_simulation *sim)
{
	int			index;
	t_slot		tmp;

	index = dongle->size;
	if (index >= 2)
		return ;
	dongle->waiting_queue[index].waiting_coder = coder;
	if (strcmp(sim->parsed->scheduler, "fifo") == 0)
		dongle->waiting_queue[index].coder_timestamps = get_current_time();
	else
		dongle->waiting_queue[index].\
coder_timestamps = coder->last_compile_time + sim->parsed->time_to_burnout;
	if (index == 1)
	{
		if (dongle->waiting_queue[index]\
.coder_timestamps < dongle->waiting_queue[0].coder_timestamps)
		{
			tmp = dongle->waiting_queue[0];
			dongle->waiting_queue[0] = dongle->waiting_queue[index];
			dongle->waiting_queue[index] = tmp;
		}
	}
	dongle->size = index + 1;
}

static t_coders	*pop_queue(t_dongles *dongle)
{
	int				index;
	t_coders		*coder;

	index = 0;
	if (dongle->size == 0)
		return (NULL);
	coder = dongle->waiting_queue[index].waiting_coder;
	if (dongle->size == 2)
	{
		dongle->waiting_queue[0] = dongle->waiting_queue[1];
		dongle->waiting_queue[1].coder_timestamps = 0;
		dongle->waiting_queue[1].waiting_coder = NULL;
	}
	else if (dongle->size == 1)
	{
		dongle->waiting_queue[0].coder_timestamps = 0;
		dongle->waiting_queue[0].waiting_coder = NULL;
	}
	dongle->size--;
	return (coder);
}

int	dongles_requeste(t_coders *coder, t_dongles *dongle, t_simulation *sim)
{
	struct timespec	tsp;

	pthread_mutex_lock(&dongle->dongle_mutex);
	push_queue(dongle, coder, sim);
	while (dongle->waiting_queue[0].waiting_coder != coder || \
dongle->release_time > get_current_time() || dongle->available == 0)
	{
		if (check_stop_flag(sim))
			return (pthread_mutex_unlock(&dongle->dongle_mutex), 0);
		tsp.tv_sec = dongle->release_time / 1000;
		tsp.tv_nsec = (dongle->release_time % 1000) * 1000000L;
		pthread_cond_timedwait(&dongle->dongle_cond, \
&dongle->dongle_mutex, &tsp);
	}
	pthread_mutex_unlock(&dongle->dongle_mutex);
	if (check_stop_flag(sim))
		return (0);
	else
	{
		pthread_mutex_lock(&dongle->dongle_mutex);
		dongle->available = 0;
		log_fct(sim, coder->id, 1);
		pop_queue(dongle);
		return (pthread_mutex_unlock(&dongle->dongle_mutex), 1);
	}
}

void	put_dongel(t_dongles *dongle, t_simulation *sim)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	dongle->available = 1;
	dongle->release_time = get_current_time() + sim->parsed->dongle_cooldown;
	pthread_cond_broadcast(&dongle->dongle_cond);
	pthread_mutex_unlock(&dongle->dongle_mutex);
}

int	take_dongles(t_coders *coder, t_simulation *simulater)
{
	if (coder->id % 2 == 0)
	{
		usleep(100);
		if (dongles_requeste(coder, coder->right_dongle, simulater) == 0)
			return (0);
		if (dongles_requeste(coder, coder->left_dongle, simulater) == 0)
			return (0);
	}
	else
	{
		if (dongles_requeste(coder, coder->left_dongle, simulater) == 0)
			return (0);
		if (dongles_requeste(coder, coder->right_dongle, simulater) == 0)
			return (0);
	}
	return (1);
}
