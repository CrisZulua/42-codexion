/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 16:46:28 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/25 12:37:22 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
		print("ERROR: Could not create monitor thread\n");
		cleanup_sim(&sim);
		return (1);
	}
	created = 0;
	i = 0;
	while (i < sim.nb_coders)
	{
		if (pthread_create(&sim.coders[i].thread, NULL,
						   coder_routine, &sim.coders[i]) != 0)
		{
			printf("ERROR: Could not create thread for coder %d\n", i + 1);
			stop_simulation(&sim);
			break;
		}
		created++;
		i++;
	}
	pthread_join(sim.monitor, NULL);
	i = 0;
	while (i < created)
	{
		pthread_join(sim.coders[i].thread, NULL);
		i++;
	}
	if (all_coders_done(&sim))
		printf("\nTodos los coders llegaron a %d compilaciones. "
			   "Fin de la simulacion.\n",
			   sim.compiles_required);
	cleanup_sim(&sim);
	return (0);
}
