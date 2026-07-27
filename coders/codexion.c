/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 16:46:28 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/27 12:25:48 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int create_coders_threads(t_sim *sim)
{
	int i;
	int created;

	created = 0;
	i = 0;
	while (i < sim->nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
						   coder_routine, &sim->coders[i]) != 0)
		{
			printf("ERROR: Could not create thread for coder %d\n", i + 1);
			stop_sim(sim);
			break;
		}
		created++;
		i++;
	}
	return created;
}

int main(int argc, char **argv)
{
	t_sim sim;
	int i;
	int created;

	if (!parse_args(argc, argv))
		return (1);
	if (!init_sim(&sim, argv))
		return (1);
	if (pthread_create(&sim.monitor, NULL, monitor_routine, &sim) != 0)
	{
		printf("ERROR: Could not create monitor thread\n");
		cleanup_sim(&sim);
		return (1);
	}
	created = create_coders_threads(&sim);
	pthread_join(sim.monitor, NULL);
	i = 0;
	while (i < created)
	{
		pthread_join(sim.coders[i].thread, NULL);
		i++;
	}
	cleanup_sim(&sim);
	return (0);
}
