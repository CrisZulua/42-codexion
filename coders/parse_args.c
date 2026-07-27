/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 14:53:09 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/27 18:14:42 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "parse_args.h"

int	is_positive(char *number)
{
	int	i;

	i = 0;
	if (number[i] == '-')
		return (0);
	if (number[i] == '+')
		i++;
	while (i < (int)strlen(number))
	{
		if (number[i] < '0' || number[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	parse_args(int argc, char **argv)
{
	int	i;

	if (argc != 9)
	{
		printf("%s\n", NOT_ENOUGH_ARGS);
		printf("usage: ./codexion <n_coders> <time_to_burnout> ");
		printf("<time_to_compile> <time_to_debug> <time_to_refactor>");
		printf(" <n_compiles_required> <dongle_cooldown> ");
		printf("<scheduler(fifo|edf)>\n");
		return (0);
	}
	i = 1;
	while (i < 8)
	{
		if (!is_positive(argv[i]))
			return (printf("%s\n", BAD_NUMBER_VALUE), 0);
		i++;
	}
	if (strcmp(argv[i], "fifo") != 0 && strcmp(argv[i], "edf") != 0)
		return (printf("%s\n", UNKNOWN_SCHEDULER), 0);
	return (1);
}
