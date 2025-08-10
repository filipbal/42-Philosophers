/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbalakov <fbalakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:57:20 by fbalakov          #+#    #+#             */
/*   Updated: 2025/08/10 18:34:21 by fbalakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Check if string is a valid positive integer */
static int	is_valid_number(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/* Check if arguments are valid positive integers */
static int	validate_args(char **argv)
{
	int	i;
	int	value;

	i = 1;
	while (argv[i])
	{
		if (!is_valid_number(argv[i]))
			return (0);
		value = ft_atoi(argv[i]);
		if (value <= 0)
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
		printf("./philo num_philos t_die t_eat t_sleep [must_eat_count]\n");
		return (1);
	}
	if (!validate_args(argv))
	{
		printf("Error: Invalid args. All values must be positive integers.\n");
		return (1);
	}
	if (!init_data(&data, argv) || !init_philos(&data)
		|| !start_simulation(&data))
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
