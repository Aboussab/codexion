/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboussab <aboussab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:13:42 by aboussab          #+#    #+#             */
/*   Updated: 2026/08/09 19:07:49 by aboussab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	error_join(char *err)
{
	fprintf(stderr, "UNFortunately there is an error: %s\n", err);
}

void	log_fct(t_simulation *monitor, int index, int n)
{
	long long		timestamp;

	timestamp = get_current_time() - monitor->start_time;
	pthread_mutex_lock(&monitor->log_mutex);
	if (n == 1 && !check_stop_flag(monitor))
		printf("%lld %d has taken a dongle\n"\
, timestamp, monitor->all_coders[index - 1].id);
	else if (n == 2 && !check_stop_flag(monitor))
		printf("%lld %d is compiling\n"\
, timestamp, monitor->all_coders[index - 1].id);
	else if (n == 3 && !check_stop_flag(monitor))
		printf("%lld %d is debugging\n"\
, timestamp, monitor->all_coders[index - 1].id);
	else if (n == 4 && !check_stop_flag(monitor))
		printf("%lld %d is refactoring\n"\
, timestamp, monitor->all_coders[index - 1].id);
	else if (n == 5 && check_stop_flag(monitor))
		printf("%lld %d burned out\n"\
, timestamp, monitor->all_coders[index - 1].id);
	pthread_mutex_unlock(&monitor->log_mutex);
}
