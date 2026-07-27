/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_routines.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 11:45:54 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/27 12:24:30 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void free_dongle(t_sim *sim, int pos)
{
	long long current_time;
	current_time = get_ms();
	sim->dongles[pos].available_at_ms = current_time + sim->dongle_cd_time_ms;
	sim->dongles[pos].active = FALSE;
}

static void take_dongles(t_sim *sim, int id)
{
	int right;

	right = (id + 1) % sim->nb_coders;
	sim->dongles[id].active = TRUE;
	sim->dongles[right].active = TRUE;
}

void realease_dongles(t_sim *sim, int id)
{
	int pos;
	pthread_mutex_lock(&sim->lock);
	// Left dongle
	pos = id;
	free_dongle(sim, pos);
	// Rigth dongle
	pos = (id + 1) % sim->nb_coders;
	free_dongle(sim, pos);
	pthread_cond_broadcast(&sim->dongles_freed);
	pthread_mutex_unlock(&sim->lock);
}

static t_bool dongles_free(t_coder *coder)
{
	t_sim *sim;
	t_bool left;
	t_bool right;
	int right_pos;
	long long time;

	sim = coder->sim;
	time = get_ms();
	right_pos = (coder->id + 1) % sim->nb_coders;
	left = sim->dongles[coder->id].active;
	if (time < sim->dongles[coder->id].available_at_ms)
		return FALSE;
	right = sim->dongles[right_pos].active;
	if (time < sim->dongles[right_pos].available_at_ms)
		return FALSE;
	return (left && right);
}

void request_dongles(t_coder *coder)
{
	pthread_mutex_t *lock;
	pthread_cond_t *dongles_freed;
	struct timespec time;

	lock = &coder->sim->lock;
	dongles_freed = &coder->sim->dongles_freed;
	pthread_mutex_lock(lock);
	get_in_queue(coder);
	while (coder->sim->running && !(my_turn(coder) && dongles_free(coder)))
	{
		get_timeout(&time, 5);
		pthread_cond_timedwait(dongles_freed, lock, &time);
	}
	if (coder->sim->running)
	{
		heap_pop(coder->sim);
		take_dongles(coder->sim, coder->id);
	}
	pthread_mutex_unlock(&coder->sim->lock);
}
