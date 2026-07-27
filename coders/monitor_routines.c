/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routines.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 18:43:30 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/27 18:32:40 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_bool	is_burned_out(t_coder *coder)
{
	t_bool	state;

	pthread_mutex_lock(&coder->burnout_mx);
	state = get_ms() >= coder->to_bournout_ms;
	pthread_mutex_unlock(&coder->burnout_mx);
	return (state);
}

void	stop_sim(t_sim *sim)
{
	pthread_mutex_lock(&sim->lock);
	sim->running = FALSE;
	pthread_mutex_unlock(&sim->lock);
}

t_bool	coders_finished(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		if (!is_finished(&sim->coders[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static t_bool	any_burned_out(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		if (is_burned_out(&sim->coders[i]))
		{
			print_log(sim, i, "burned out");
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (sim_is_running(sim))
	{
		pthread_mutex_lock(&sim->lock);
		if (any_burned_out(sim))
		{
			stop_sim(sim);
			pthread_mutex_unlock(&sim->lock);
			return (NULL);
		}
		if (coders_finished(sim))
		{
			stop_sim(sim);
			pthread_mutex_unlock(&sim->lock);
			return (NULL);
		}
		pthread_mutex_unlock(&sim->lock);
		usleep(1000);
	}
	return (NULL);
}
