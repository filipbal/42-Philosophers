/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbalakov <fbalakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:57:17 by fbalakov          #+#    #+#             */
/*   Updated: 2025/08/10 18:50:06 by fbalakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Initialize main data structure with command line arguments */
int	init_data(t_data *data, char **argv)
{
	memset(data, 0, sizeof(t_data));
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;
	data->simulation_end = 0;
	if (data->num_philos < 1
		|| data->time_to_die < 1
		|| data->time_to_eat < 1
		|| data->time_to_sleep < 1)
		return (0);
	if (argv[5] && data->must_eat_count < 1)
		return (0);
	return (1);
}

/* Clean up fork mutexes on initialization failure */
static void	cleanup_fork_init(t_data *data, int count)
{
	int	i;

	i = 0;
	while (i < count)
		pthread_mutex_destroy(&data->forks[i++]);
	free(data->forks);
	data->forks = NULL;
}

/* Initialize mutex for each fork */
static int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			cleanup_fork_init(data, i);
			return (0);
		}
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0
		|| pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		cleanup_fork_init(data, data->num_philos);
		return (0);
	}
	return (1);
}

/* Initialize philosopher structures and assign forks */
int	init_philos(t_data *data)
{
	int	i;

	if (!init_forks(data))
		return (0);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = 0;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1)
			% data->num_philos];
		data->philos[i].data = data;
		i++;
	}
	return (1);
}

/* Clean up allocated memory and destroy mutexes */
void	cleanup(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->num_philos)
			pthread_mutex_destroy(&data->forks[i++]);
		free(data->forks);
		data->forks = NULL;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
}
