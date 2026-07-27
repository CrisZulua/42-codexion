/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_routines_b.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:26:16 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/27 18:26:35 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_bool	sim_is_running(t_sim *sim)
{
	t_bool	status;

	pthread_mutex_lock(&sim->lock);
	status = sim->running;
	pthread_mutex_unlock(&sim->lock);
	return (status);
}

void	cleanup_sim(t_sim *sim)
{
	int	i;

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
