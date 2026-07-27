/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 14:59:16 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/27 18:13:10 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_ARGS_H
# define PARSE_ARGS_H

# define NOT_ENOUGH_ARGS "[ERROR] 9 arguments required: "
# define BAD_NUMBER_VALUE "Argument type has to be a positive integer"
# define UNKNOWN_SCHEDULER "Scheduler: <fifo | edf>"

/*
	DESCRIPTION
	Checks wether the string given as argument represents a positive number.
	Only admits '+' at the beginning of the string.

	RETURN VALUE
	1 if falls into the category described.
	0 otherwise.
*/
int	is_positive(char *number);

/*
	DESCRIPTION
	Checks correct format on every argument give.

	Positive numbers, ARGS 0-7:
		number_of_coders time_to_burnout time_to_compile time_to_debug
		time_to_refactor number_of_compiles_required dongle_cooldown

	String ("fifo" | "edf") ARG 8:
		scheduler

	If there is an error, a custom message gets printed.

	RETURN VALUE
	1 if everything is ok with the arguments
	0 if there is an error on any argument
*/
int	parse_args(int argc, char **argv);

#endif
