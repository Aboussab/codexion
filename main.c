/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboussab <aboussab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 19:18:28 by aboussab          #+#    #+#             */
/*   Updated: 2026/08/10 00:40:54 by aboussab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_simulation(t_simulation *simulater, int i)
{
	if ((pthread_create(&(simulater->monitor), \
NULL, detect_burn_out, simulater)) != 0)
		return (error_join("errore while creating simuleter thread."), 0);
	while (i < simulater->parsed->number_of_coders)
	{
		if ((pthread_create(&(simulater->all_coders[i].coder_thread), \
NULL, coder_routine, &(simulater->all_coders[i]))) != 0)
			return (error_join("errore while creating coders."), 0);
		i++;
	}
	pthread_mutex_lock(&simulater->flag_mutex);
	simulater->start_flag = 1;
	pthread_mutex_unlock(&simulater->flag_mutex);
	pthread_cond_broadcast(&simulater->start_simulation);
	i = 0;
	while (i < simulater->parsed->number_of_coders)
	{
		if (pthread_join(simulater->all_coders[i].coder_thread, NULL) != 0)
			return (error_join("errore while joining coders."), 0);
		i++;
	}
	if (pthread_join(simulater->monitor, NULL) != 0)
		return (error_join("errore while joining simulleter thread."), 0);
	return (ft_distroy(simulater), 1);
}

int	main(int argc, char **argv)
{
	t_parse			*arg;
	t_simulation	*simulater;
	int				i;

	i = 0;
	arg = ft_parse(argc, argv);
	if (!arg)
		return (0);
	simulater = inisialize_simulater(arg);
	if (!simulater)
		return (error_join("missing sumilation"), 0);
	start_simulation(simulater, i);
	return (0);
}
