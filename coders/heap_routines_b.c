/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_routines_b.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 11:20:54 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/27 11:42:23 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void get_in_queue(t_coder *coder)
{
	t_sim *sim;

	sim = coder->sim;
	coder->waiting_start_ms = get_ms();
	heap_push(sim, coder);
}

t_bool my_turn(t_coder *coder)
{
	t_coder *priority;

	priority = heap_peek(coder->sim);
	return (coder->id == priority->id);
}
