/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 19:19:17 by czuluaga          #+#    #+#             */
/*   Updated: 2026/08/03 12:10:12 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_bool	is_finished(t_coder *coder)
{
	t_bool	state;

	pthread_mutex_lock(&coder->ended_mx);
	state = coder->finished;
	pthread_mutex_unlock(&coder->ended_mx);
	return (state);
}

static void	finish_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->ended_mx);
	coder->finished = TRUE;
	pthread_mutex_unlock(&coder->ended_mx);
}

void	update_burnout_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->burnout_mx);
	coder->to_bournout_ms = get_ms() + coder->sim->burnout_time_ms;
	pthread_mutex_unlock(&coder->burnout_mx);
}

static void	coder_loop(t_sim *sim, t_coder *coder)
{
	while (!is_finished(coder) && !is_burned_out(coder))
	{
		request_dongles(coder);
		if (!sim_is_running(sim))
			break ;
		compile(sim, coder);
		if (!sim_is_running(sim))
			break ;
		coder->compilations++;
		realease_dongles(sim, coder->id);
		if (coder->compilations == sim->compiles_required)
			finish_coder(coder);
		if (coder->compilations == sim->compiles_required)
			break ;
		if (!sim_is_running(sim))
			break ;
		print_log(sim, coder->id, "is debugging");
		sleep_thread(sim->debug_time_ms, coder);
		if (!sim_is_running(sim))
			break ;
		print_log(sim, coder->id, "is refactoring");
		sleep_thread(sim->refactor_time_ms, coder);
		if (!sim_is_running(sim))
			break ;
	}
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	coder_loop(sim, coder);
	return (NULL);
}
