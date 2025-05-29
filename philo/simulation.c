/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbalakov <fbalakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:57:28 by fbalakov          #+#    #+#             */
/*   Updated: 2025/05/29 14:26:27 by fbalakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_simulation(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	while (i < data->args.num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor_death, data) != 0)
		return (1);
	i = 0;
	while (i < data->args.num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	return (0);
}

void	*monitor_death(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!is_simulation_end(data))
	{
		if (check_philosopher_deaths(data))
			return (NULL);
		if (check_all_ate_enough(data))
		{
			set_simulation_end(data);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

int	check_philosopher_deaths(t_data *data)
{
	int		i;
	long	current_time;

	i = 0;
	while (i < data->args.num_philos && !is_simulation_end(data))
	{
		current_time = get_time_ms();
		if (current_time - data->philos[i].last_meal_time
			>= data->args.time_to_die)
		{
			print_status(&data->philos[i], "died");
			set_simulation_end(data);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_all_ate_enough(t_data *data)
{
	int	i;

	if (!data->args.has_must_eat)
		return (0);
	i = 0;
	while (i < data->args.num_philos)
	{
		if (data->philos[i].meals_eaten < data->args.must_eat_count)
			return (0);
		i++;
	}
	return (1);
}
