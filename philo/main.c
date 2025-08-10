/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbalakov <fbalakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:57:20 by fbalakov          #+#    #+#             */
/*   Updated: 2025/08/10 18:19:38 by fbalakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Check if arguments are valid positive integers */
static int	validate_args(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		if (ft_atoi(argv[i]) <= 0)
			return (0);
		i++;
	}
	return (1);
}

/* Start simulation by creating philosopher threads */
static int	start_simulation(t_data *data)
{
	int	i;

	data->start_time = get_time();
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

/* Monitor philosophers for death or completion */
static void	monitor_simulation(t_data *data)
{
	int		i;
	int		done;
	long	now;

	while (!data->simulation_end)
	{
		i = -1;
		done = 1;
		while (++i < data->num_philos && !data->simulation_end)
		{
			now = get_time();
			if (now - data->philos[i].last_meal_time > data->time_to_die)
			{
				print_status(&data->philos[i], "died");
				data->simulation_end = 1;
				break ;
			}
			if (data->must_eat_count != -1
				&& data->philos[i].meals_eaten < data->must_eat_count)
				done = 0;
		}
		if (data->must_eat_count != -1 && done)
			data->simulation_end = 1;
		usleep(1000);
	}
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo num_philos time_die time_eat time_sleep"
			" [must_eat_count]\n");
		return (1);
	}
	if (!validate_args(argv) || !init_data(&data, argv)
		|| !init_philos(&data) || !start_simulation(&data))
	{
		cleanup(&data);
		return (1);
	}
	monitor_simulation(&data);
	i = 0;
	while (i < data.num_philos)
		pthread_join(data.philos[i++].thread, NULL);
	cleanup(&data);
	return (0);
}
