/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboussab <aboussab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/09 17:10:09 by aboussab          #+#    #+#             */
/*   Updated: 2026/08/10 03:39:01 by aboussab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>
# include <time.h>

typedef struct parsing
{
	int					number_of_coders;
	long long			time_to_burnout;
	long long			time_to_compile;
	long long			time_to_debug;
	long long			time_to_refactor;
	long long			n_compiles_required;
	long long			dongle_cooldown;
	char				*scheduler;
}	t_parse;

typedef struct coders		t_coders;
typedef struct simulation	t_simulation;

typedef struct t_waiting_slot
{
	t_coders				*waiting_coder;
	long long				coder_timestamps;
}	t_slot;

typedef struct dongles
{
	int						id;
	int						available;
	long long				release_time;
	pthread_mutex_t			dongle_mutex;
	pthread_cond_t			dongle_cond;
	t_slot					waiting_queue[2];
	int						size;
}	t_dongles;

typedef struct coders
{
	int							id;
	int							counter_compiling;
	long long					last_compile_time;
	t_dongles					*left_dongle;
	t_dongles					*right_dongle;
	pthread_mutex_t				counter_compiling_mutex;
	pthread_t					coder_thread;
	t_simulation				*manager;
}	t_coders;

typedef struct simulation
{
	t_coders					*all_coders;
	t_dongles					*all_dongles;
	t_parse						*parsed;
	pthread_mutex_t				flag_mutex;
	pthread_cond_t				start_simulation;
	int							stop_flag;
	int							start_flag;
	long long					start_time;
	pthread_mutex_t				log_mutex;
	pthread_t					monitor;
}	t_simulation;

void						error_join(char *err);
t_parse						*ft_parse(int argc, char **argv);
long						get_current_time(void);
void						log_fct(t_simulation *monitor, int i, int n);
t_simulation				*inisialize_simulater(t_parse *arg);
int							start_simulation(t_simulation *simulater, int i);
void						*coder_routine(void *arg);
int							dongles_requeste\
(t_coders *coder, t_dongles*dongle, t_simulation *sim);
void						put_dongel(t_dongles*dongle, t_simulation *sim);
int							check_stop_flag(t_simulation *simulater);
void						*detect_burn_out(void *arg);
void						ft_distroy(t_simulation *simulater);
int							take_dongles\
(t_coders *coder, t_simulation *simulater);
void						coder_doing\
(t_coders *coder, t_simulation *simulater);

#endif 