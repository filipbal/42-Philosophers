/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbalakov <fbalakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:57:30 by fbalakov          #+#    #+#             */
/*   Updated: 2025/05/29 14:14:26 by fbalakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_sleep(long duration)
{
	long	start_time;
	long	elapsed;

	start_time = get_time_ms();
	while (1)
	{
		elapsed = get_time_ms() - start_time;
		if (elapsed >= duration)
			break ;
		if (duration - elapsed > 10)
			usleep((duration - elapsed) * 500);
		else
			usleep(100);
	}
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_simulation_end(philo->data))
	{
		timestamp = get_time_ms() - philo->data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	is_simulation_end(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->death_mutex);
	result = data->simulation_end;
	pthread_mutex_unlock(&data->death_mutex);
	return (result);
}

void	set_simulation_end(t_data *data)
{
	pthread_mutex_lock(&data->death_mutex);
	data->simulation_end = 1;
	pthread_mutex_unlock(&data->death_mutex);
}