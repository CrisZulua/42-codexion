/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_routines.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 11:16:46 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/27 11:18:01 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long long heap_key(t_sim *sim, t_coder *coder)
{
	if (sim->scheduler == T_FIFO)
		return (coder->waiting_start_ms);
	return (coder->to_bournout_ms);
}

static void heap_swap(t_sim *sim, int a, int b)
{
	t_coder *tmp;

	tmp = sim->heap[a];
	sim->heap[a] = sim->heap[b];
	sim->heap[b] = tmp;
}

/*
	Mira el elemento con mayor prioridad (clave mas pequena) sin
	sacarlo del heap. O(1). Devuelve NULL si el heap esta vacio.
*/
t_coder *heap_peek(t_sim *sim)
{
	if (sim->heap_size == 0)
		return (NULL);
	return (sim->heap[0]);
}

/*
	Inserta un coder en el heap y restaura la propiedad de heap
	"burbujeando" hacia arriba: lo compara con su padre y va
	intercambiando mientras el nuevo elemento tenga mejor prioridad
	(clave mas pequena) que su padre. O(log n).
*/
void heap_push(t_sim *sim, t_coder *coder)
{
	int i;
	int parent;

	if (sim->heap_size >= sim->nb_coders)
		return;
	sim->heap[sim->heap_size] = coder;
	i = sim->heap_size;
	sim->heap_size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (heap_key(sim, sim->heap[parent]) <= heap_key(sim, sim->heap[i]))
			break;
		heap_swap(sim, parent, i);
		i = parent;
	}
}

/*
	Saca y devuelve el elemento con mayor prioridad (la raiz).
	Mueve el ultimo elemento del heap a la raiz y restaura la
	propiedad de heap "burbujeando" hacia abajo: en cada paso se
	compara con sus dos hijos y se intercambia con el menor de
	ellos, hasta que ya no haya ningun hijo con mejor prioridad.
	O(log n). Devuelve NULL si el heap esta vacio.
*/
t_coder *heap_pop(t_sim *sim)
{
	t_coder *top;
	int i;
	int left;
	int right;
	int smallest;

	if (sim->heap_size == 0)
		return (NULL);
	top = sim->heap[0];
	sim->heap_size--;
	sim->heap[0] = sim->heap[sim->heap_size];
	i = 0;
	while (TRUE)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < sim->heap_size &&
			heap_key(sim, sim->heap[left]) < heap_key(sim, sim->heap[smallest]))
			smallest = left;
		if (right < sim->heap_size &&
			heap_key(sim, sim->heap[right]) < heap_key(sim, sim->heap[smallest]))
			smallest = right;
		if (smallest == i)
			break;
		heap_swap(sim, i, smallest);
		i = smallest;
	}
	return (top);
}
