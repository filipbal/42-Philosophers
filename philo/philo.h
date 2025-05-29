/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbalakov <fbalakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:57:24 by fbalakov          #+#    #+#             */
/*   Updated: 2025/05/29 14:27:23 by fbalakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

/* Program arguments structure */
typedef struct s_args
{
	int	num_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat_count;
	int	has_must_eat;
}	t_args;

/* Individual philosopher structure */
typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	struct s_data	*data;
}	t_philo;

/* Shared program data */
typedef struct s_data
{
	t_args			args;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	long			start_time;
	int				simulation_end;
	int				all_ate_enough;
}	t_data;

/* Function prototypes */
/* main.c */
int		main(int argc, char **argv);
int		parse_arguments(int argc, char **argv, t_args *args);
void	cleanup_and_exit(t_data *data, int exit_code);

/* init.c */
int		init_data(t_data *data, t_args *args);
int		init_mutexes(t_data *data);
int		init_philosophers(t_data *data);

/* simulation.c */
int		start_simulation(t_data *data);
void	*monitor_death(void *arg);
int		check_philosopher_deaths(t_data *data);
int		check_all_ate_enough(t_data *data);

/* philosopher.c */
void	*philosopher_routine(void *arg);
void	philo_eat(t_philo *philo);
void	philo_sleep_and_think(t_philo *philo);

/* utils.c */
long	get_time_ms(void);
void	precise_sleep(long duration);
void	print_status(t_philo *philo, char *status);
int		is_simulation_end(t_data *data);
void	set_simulation_end(t_data *data);

#endif
