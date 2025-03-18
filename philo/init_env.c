/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:07:08 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 16:07:10 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file init_env.c
 * @brief Environment initialization functions for philosopher simulation.
 */
#include "philo.h"

static int	fillup_philos(t_env *env)
{
	int	i;

	i = 0;
	env->start_time = get_time();
	if (env->start_time == -1)
	{
		print_error("Error: fillup_philos: get_time failed.\n");
		return (EXIT_FAILURE);
	}
	while (i < env->num_philo)
	{
		env->philos[i].id = i;
		env->philos[i].meals = 0;
		env->philos[i].last_meal = env->start_time;
		env->philos[i].env = env;
		env->philos[i].num_philo = env->num_philo;
		env->philos[i].die_time = env->die_time;
		env->philos[i].eat_time = env->eat_time;
		env->philos[i].sleep_time = env->sleep_time;
		env->philos[i].meals_limit = env->meals_limit;
		i++;
	}
	return (EXIT_SUCCESS);
}

static int	init_mutexes(t_env *env)
{
	if (init_print_mutex(env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_meal_mutex(env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_start_mutex(env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_end_mutex(env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_forks_mutex(env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_log_buffer_mutex(env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	init_forks_philos(t_env *env)
{
	env->forks = malloc(env->num_philo * sizeof(pthread_mutex_t));
	if (!env->forks)
	{
		print_error ("Error: init_forks_philos: forks mem alloc failed.\n");
		return (EXIT_FAILURE);
	}
	env->philos = malloc(env->num_philo * sizeof(t_philo));
	if (!env->philos)
	{
		print_error ("Error: init_forks_philos: philos mem alloc failed.\n");
		free(env->forks);
		env->forks = NULL;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	init_env(t_env *env, int ac, char **av)
{
	env->num_philo = ft_atoi(av[1]);
	env->die_time = ft_atoi(av[2]);
	env->eat_time = ft_atoi(av[3]);
	env->sleep_time = ft_atoi(av[4]);
	env->meals_limit = -1;
	if (ac == 6)
		env->meals_limit = ft_atoi(av[5]);
	env->ended = 0;
	env->start_time = 0;
	env->log_buffer.count = 0;
	env->t_philos_created = false;
	env->t_logger_created = false;
	env->t_mon_created = false;
	env->philos = NULL;
	env->forks = NULL;
	if (init_forks_philos(env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_mutexes(env) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (fillup_philos(env) == EXIT_FAILURE)
	{
		destroy_mutexes(env);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
