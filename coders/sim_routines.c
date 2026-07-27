/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_routines.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 17:04:31 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/27 11:09:20 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_bool sim_is_running(t_sim *sim)
{
	t_bool status;
	pthread_mutex_lock(&sim->lock);
	status = sim->running;
	pthread_mutex_unlock(&sim->lock);
	return status;
}

void cleanup_sim(t_sim *sim)
{
	int i;

	pthread_mutex_destroy(&sim->lock);
	pthread_mutex_destroy(&sim->print_lock);
	pthread_cond_destroy(&sim->dongles_freed);
	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_destroy(&sim->coders[i].burnout_mx);
		pthread_mutex_destroy(&sim->coders[i].ended_mx);
		i++;
	}
	free(sim->coders);
	free(sim->dongles);
	free(sim->heap);
}

static void init_coders(t_sim *sim)
{
	int i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->coders[i].id = i;
		sim->coders[i].compilations = 0;
		sim->coders[i].finished = FALSE;
		sim->coders[i].to_bournout_ms = sim->start_ms + sim->burnout_time_ms;
		sim->coders[i].waiting_start_ms = sim->start_ms;
		sim->coders[i].sim = sim;
		pthread_mutex_init(&sim->coders[i].burnout_mx, NULL);
		pthread_mutex_init(&sim->coders[i].ended_mx, NULL);
		i++;
	}
}

static void init_dongles(t_sim *sim)
{
	int i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->dongles[i].available_at_ms = 0;
		sim->dongles[i].active = FALSE;
		i++;
	}
}

static void init_heap(t_sim *sim)
{
	int i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->heap[i] = NULL;
		i++;
	}
}

int init_sim(t_sim *sim, char **argv)
{
	sim->heap_size = 0;
	sim->nb_coders = atoi(argv[1]);
	sim->burnout_time_ms = atoi(argv[2]);
	sim->compile_time_ms = atoi(argv[3]);
	sim->debug_time_ms = atoi(argv[4]);
	sim->refactor_time_ms = atoi(argv[5]);
	sim->compiles_required = atoi(argv[6]);
	sim->dongle_cd_time_ms = atoi(argv[7]);
	sim->scheduler = T_FIFO;
	if (!strcmp(argv[8], "edf"))
		sim->scheduler = T_EDF;
	sim->coders = malloc(sizeof(t_coder) * (size_t)sim->nb_coders);
	sim->dongles = malloc(sizeof(t_dongle) * (size_t)sim->nb_coders);
	sim->heap = (t_coder **)malloc(sizeof(t_coder *) * sim->nb_coders);
	if (!sim->coders || !sim->dongles || !sim->heap)
	{
		free(sim->coders);
		free(sim->dongles);
		free(sim->heap);
		printf("Error: Could not initialize coders and dongles (malloc)\n");
		return (0);
	}
	pthread_mutex_init(&sim->lock, NULL);
	pthread_mutex_init(&sim->print_lock, NULL);
	pthread_cond_init(&sim->dongles_freed, NULL);
	sim->running = TRUE;
	sim->start_ms = get_ms();
	init_coders(sim);
	init_dongles(sim);
	init_heap(sim);
	return (1);
}
