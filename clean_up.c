/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboussab <aboussab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 14:59:43 by aboussab          #+#    #+#             */
/*   Updated: 2026/08/09 19:06:11 by aboussab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_distroy(t_simulation *simulater)
{
	int	i;

	i = 0;
	while (i <= simulater->parsed->number_of_coders - 1)
	{
		pthread_mutex_destroy(&(simulater->all_dongles[i].dongle_mutex));
		pthread_cond_destroy(&(simulater->all_dongles[i].dongle_cond));
		pthread_mutex_destroy\
(&((simulater->all_coders[i]).counter_compiling_mutex));
		i++;
	}
	pthread_mutex_destroy(&(simulater->flag_mutex));
	pthread_mutex_destroy(&(simulater->log_mutex));
	pthread_cond_destroy(&simulater->start_simulation);
	free(simulater->all_dongles);
	free(simulater->all_coders);
	free(simulater->parsed);
	free(simulater);
}
