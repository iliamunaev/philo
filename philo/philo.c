/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:10:43 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 13:20:00 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
* @file philo.c
* @brief Philosopher routine implementation.
*/
#include "philo.h"

/**
* @brief Arranges philosophers for thinking before starting routine.
*
* Handles initial thinking time for certain philosophers to avoid contention.
*
* @param p Pointer to the philosopher structure.
*/
static void	self_arrange(t_philo *p)
{
	bool is_odd_philo;
	bool is_even_philo;

	is_odd_philo = (p->env->num_philo & 1);
	is_even_philo = !(p->env->num_philo & 1);
	if (is_odd_philo && p->id == 0)
	{
		print_status(p, "is thinking");
		precise_sleep(p->env->eat_time << 1);
	}
	if (is_odd_philo&& (p->id & 1))
	{
		print_status(p, "is thinking");
		precise_sleep(p->env->eat_time);
	}
	else if (is_even_philo && p->id & 1)
	{
		print_status(p, "is thinking");
		precise_sleep(p->env->eat_time);
	}
}

/**
 * @brief Executes the repeated routine of a philosopher.
 *
 * This includes taking forks, eating, sleeping, and thinking.
 *
 * @param p Pointer to the philosopher structure.
 */
static void	repeat_routine(t_philo *p)
{
	take_forks(p);
	pthread_mutex_lock(&p->env->meal_mutex);
	p->last_meal = get_time();
	pthread_mutex_unlock(&p->env->meal_mutex);
	print_status(p, "is eating");
	precise_sleep(p->env->eat_time);
	pthread_mutex_lock(&p->env->meal_mutex);
	p->meals++;
	pthread_mutex_unlock(&p->env->meal_mutex);
	put_forks(p);
	print_status(p, "is sleeping");
	precise_sleep(p->env->sleep_time);
	print_status(p, "is thinking");
}

/**
 * @brief Handles the case where there is only one philosopher.
 *
 * Since a single philosopher cannot acquire two forks, they just
 * think until they die.
 *
 * @param p Pointer to the philosopher structure.
 */
static void	process_single_philo(t_philo *p)
{
	print_status(p, "is thinking");
	usleep(p->env->die_time + 5);
}

static void wait_all_threads(t_philo *p)
{
	pthread_mutex_lock(&p->env->start_mutex);
	while (get_time() < p->env->start_time)
		usleep(50);
	pthread_mutex_unlock(&p->env->start_mutex);
	pthread_mutex_lock(&p->env->end_mutex);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	wait_all_threads(p);
	if (p->env->num_philo == 1)
	{
		process_single_philo(p);
		pthread_mutex_unlock(&p->env->end_mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&p->env->end_mutex);
	self_arrange(p);
	while (1)
	{
		pthread_mutex_lock(&p->env->end_mutex);
		if (p->env->ended
			|| (p->env->meals_limit != -1 && p->meals >= p->env->meals_limit))
		{
			pthread_mutex_unlock(&p->env->end_mutex);
			break ;
		}
		pthread_mutex_unlock(&p->env->end_mutex);
		repeat_routine(p);
		usleep(500);
	}
	return (NULL);
}
