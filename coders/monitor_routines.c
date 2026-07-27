/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routines.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 18:43:30 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/27 12:57:52 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_bool is_burned_out(t_coder *coder)
{
	t_bool state;

	pthread_mutex_lock(&coder->burnout_mx);
	state = get_ms() >= coder->to_bournout_ms;
	pthread_mutex_unlock(&coder->burnout_mx);
	return state;
}

void stop_sim(t_sim *sim)
{
	pthread_mutex_lock(&sim->lock);
	sim->running = FALSE;
	pthread_mutex_unlock(&sim->lock);
}

t_bool coders_finished(t_sim *sim)
{
	int i;

	i = 0;
	while (i < sim->nb_coders)
	{
		if (!is_finished(&sim->coders[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

void *monitor_routine(void *arg)
{
	t_sim *sim;
	int i;

	sim = (t_sim *)arg;
	while (sim_is_running(sim))
	{
		i = 0;
		pthread_mutex_lock(&sim->lock);
		while (i < sim->nb_coders)
		{
			if (is_burned_out(&sim->coders[i]))
			{
				pthread_mutex_unlock(&sim->lock);
				print_log(sim, i + 1, "burned out");
				stop_sim(sim);
				return (NULL);
			}
			i++;
		}
		if (coders_finished(sim))
		{
			pthread_mutex_unlock(&sim->lock);
			stop_sim(sim);
			return (NULL);
		}
		pthread_mutex_unlock(&sim->lock);
		usleep(1000);
	}
	return (NULL);
}
