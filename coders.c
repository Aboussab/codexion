/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboussab <aboussab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 15:34:38 by aboussab          #+#    #+#             */
/*   Updated: 2026/08/09 19:06:47 by aboussab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	coder_is_compiling(t_coders *coder, t_simulation *simulater)
{
	pthread_mutex_lock(&coder->counter_compiling_mutex);
	coder->last_compile_time = get_current_time();
	pthread_mutex_unlock(&coder->counter_compiling_mutex);
	log_fct(simulater, coder->id, 2);
	usleep(simulater->parsed->time_to_compile * 1000);
}

static void	coder_debbuging(t_simulation *simulater, t_coders *coder)
{
	log_fct(simulater, coder->id, 3);
	usleep(simulater->parsed->time_to_debug * 1000);
}

static void	coder_refactoring(t_simulation *simulater, t_coders *coder)
{
	log_fct(simulater, coder->id, 4);
	usleep(simulater->parsed->time_to_refactor * 1000);
}

void	coder_doing(t_coders *coder, t_simulation *simulater)
{
	coder_is_compiling(coder, simulater);
	put_dongel(coder->right_dongle, simulater);
	put_dongel(coder->left_dongle, simulater);
	coder_debbuging(simulater, coder);
	coder_refactoring(simulater, coder);
}
