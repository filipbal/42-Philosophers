/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbalakov <fbalakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:57:26 by fbalakov          #+#    #+#             */
/*   Updated: 2025/05/29 14:23:28 by fbalakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		precise_sleep(1);
	while (!is_simulation_end(philo->data))
	{
		philo_eat(philo);
		if (is_simulation_end(philo->data))
			break ;
		philo_sleep_and_think(philo);
	}
	return (NULL);
}

void	philo_eat(t_philo *philo)
{
	int	left_fork;
	int	right_fork;

	left_fork = philo->id - 1;
	right_fork = philo->id % philo->data->args.num_philos;
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(&philo->data->forks[left_fork]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[right_fork]);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[right_fork]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[left_fork]);
		print_status(philo, "has taken a fork");
	}
	print_status(philo, "is eating");
	philo->last_meal_time = get_time_ms();
	philo->meals_eaten++;
	precise_sleep(philo->data->args.time_to_eat);
	pthread_mutex_unlock(&philo->data->forks[left_fork]);
	pthread_mutex_unlock(&philo->data->forks[right_fork]);
}

void	philo_sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_sleep(philo->data->args.time_to_sleep);
	print_status(philo, "is thinking");
}
