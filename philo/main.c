/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbalakov <fbalakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:57:20 by fbalakov          #+#    #+#             */
/*   Updated: 2025/05/29 14:04:02 by fbalakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;
	t_args	args;

	if (parse_arguments(argc, argv, &args) != 0)
	{
		printf("Usage: %s num_philos time_to_die time_to_eat ", argv[0]);
		printf("time_to_sleep [must_eat_count]\n");
		return (1);
	}
	if (init_data(&data, &args) != 0)
	{
		printf("Error: Failed to initialize data\n");
		return (1);
	}
	if (start_simulation(&data) != 0)
	{
		printf("Error: Simulation failed\n");
		cleanup_and_exit(&data, 1);
	}
	cleanup_and_exit(&data, 0);
	return (0);
}

int	parse_arguments(int argc, char **argv, t_args *args)
{
	if (argc < 5 || argc > 6)
		return (1);
	args->num_philos = atoi(argv[1]);
	args->time_to_die = atoi(argv[2]);
	args->time_to_eat = atoi(argv[3]);
	args->time_to_sleep = atoi(argv[4]);
	if (args->num_philos <= 0 || args->time_to_die <= 0
		|| args->time_to_eat <= 0 || args->time_to_sleep <= 0)
		return (1);
	if (argc == 6)
	{
		args->must_eat_count = atoi(argv[5]);
		if (args->must_eat_count <= 0)
			return (1);
		args->has_must_eat = 1;
	}
	else
		args->has_must_eat = 0;
	return (0);
}

void	cleanup_and_exit(t_data *data, int exit_code)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->args.num_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	if (data->philos)
		free(data->philos);
	exit(exit_code);
}