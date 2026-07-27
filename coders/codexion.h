/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 16:04:46 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/27 10:41:35 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

#define _XOPEN_SOURCE 500
#include <pthread.h>
#include "parse_args.h"
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef enum e_scheduler
{
	T_FIFO,
	T_EDF
} t_scheduler;

#define TRUE 1
#define FALSE 0
typedef char t_bool;

typedef struct s_dongle
{
	long long available_at_ms;
	t_bool active;
} t_dongle;

struct s_sim;

typedef struct s_coder
{
	int id;
	int compilations;
	t_bool finished;
	long long waiting_start_at_ms;
	long long to_bournout_ms;
	// Mutex
	pthread_mutex_t burnout_mx;
	pthread_mutex_t ended_mx;

	pthread_t thread;
	struct s_sim *sim;
} t_coder;

typedef struct s_queue
{
	t_coder *coder;
	struct s_queue *next;
} t_queue;

typedef struct s_sim
{
	// Simulation data
	int nb_coders;
	int compiles_required;
	long long burnout_time_ms;
	long long compile_time_ms;
	long long debug_time_ms;
	long long refactor_time_ms;
	long long dongle_cd_time_ms;
	t_scheduler scheduler;

	// Objects data
	t_coder *coders;
	t_dongle *dongles;
	t_queue *queue;

	// Mutex
	pthread_mutex_t lock;
	pthread_mutex_t print_lock;

	// Monitor thread
	pthread_t monitor;
	pthread_cond_t dongles_freed;

	t_bool running;
	long long start_ms;
} t_sim;

// Time functions
long long get_ms();
void get_timeout(struct timespec *ts, long long delay_ms);

// Mutex routines
t_bool is_burned_out(t_coder *coder);
t_bool is_finished(t_coder *coder);

// Dongles routine
void realease_dongles(t_sim *sim, int id);
void request_dongles(t_coder *coder);

// Thread routines
void *monitor_routine(void *arg);
void *coder_routine(void *arg);
void sleep_thread(long long ms, t_coder *coder);

// Sim functions
int init_sim(t_sim *sim, char **argv);
void cleanup_sim(t_sim *sim);
t_bool sim_is_running(t_sim *sim);
void print_log(t_sim *sim, int id, const char *msg);
void stop_sim(t_sim *sim);
t_bool coders_finished(t_sim *sim);

#endif
