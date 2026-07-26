/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_a.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czuluaga <czuluaga@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 12:18:13 by czuluaga          #+#    #+#             */
/*   Updated: 2026/07/26 12:47:54 by czuluaga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void print_log(t_sim *sim, int id, const char *msg)
{
	long long ts;

	ts = get_ms() - sim->start_ms;
	pthread_mutex_lock(&sim->print_lock);
	printf("%lld %d  %s\n", ts, id, msg);
	pthread_mutex_unlock(&sim->print_lock);
}

void sleep_thread(long long ms, t_coder *coder)
{
	long long slept;
	long long step;

	slept = 0;
	while (slept < ms && sim_is_running(coder->sim))
	{
		step = 5;
		if (ms - slept < step)
			step = ms - slept;
		usleep(step * 1000);
		slept += step;
	}
}

long long get_ms()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void get_timeout(struct timespec *ts, long long delay_ms)
{
	struct timeval now;
	long long total_usec;

	gettimeofday(&now, NULL);
	total_usec = (long long)now.tv_sec * 1000000 + now.tv_usec + delay_ms * 1000;
	ts->tv_sec = total_usec / 1000000;
	ts->tv_nsec = (total_usec % 1000000) * 1000;
}
