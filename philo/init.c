/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbalakov <fbalakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:57:17 by fbalakov          #+#    #+#             */
/*   Updated: 2025/05/29 14:23:15 by fbalakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, t_args *args)
{
	data->args = *args;
	data->philos = NULL;
	data->forks = NULL;
	data->start_time = get_time_ms();
	data->simulation_end = 0;
	data->all_ate_enough = 0;
	if (init_mutexes(data) != 0)
		return (1);
	if (init_philosophers(data) != 0)
		return (1);
	return (0);
}

int	init_mutexes(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->args.num_philos);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->args.num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	init_philosophers(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->args.num_philos);
	if (!data->philos)
		return (1);
	i = 0;
	while (i < data->args.num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].data = data;
		i++;
	}
	return (0);
}
