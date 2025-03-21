/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_flusher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:40:04 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 21:13:21 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file log_flusher.c
 * @brief Handles buffered logging for philosopher simulation.
 *
 * This file contains functions for logging philosopher actions in a thread-safe
 * manner using a log buffer. The `log_flusher` thread continuously processes
 * and flushes log entries to standard output.
 */

#include "philo.h"

/**
 * @brief Adds a log entry to the log buffer.
 *
 * This function stores philosopher events in a shared log buffer. It ensures
 * thread safety by locking the buffer while adding new entries.
 *
 * @param buf Pointer to the log buffer.
 * @param timestamp Time at which the event occurred.
 * @param id Philosopher ID.
 * @param status Status message of the philosopher.
 */
void	buffered_print(t_log_buffer *buf,
					long timestamp,
					int id,
					const char *status)
{
	pthread_mutex_lock(&buf->mutex);
	if (buf->count < LOG_BUFFER_SIZE)
	{
		buf->entries[buf->count].timestamp = timestamp;
		buf->entries[buf->count].id = id;
		ft_strncpy(buf->entries[buf->count].status, status,
			sizeof(buf->entries[buf->count].status) - 1);
		buf->entries[buf->count].status[
			sizeof(buf->entries[buf->count].status) - 1] = '\0';
		buf->count++;
	}
	pthread_mutex_unlock(&buf->mutex);
}

/**
 * @brief Logs the current status of a philosopher.
 *
 * This function records the philosopher's state (e.g., eating, thinking,
 * sleeping)
 * by capturing the event timestamp relative to the simulation start time and
 * storing it in the log buffer.
 *
 * @param p Pointer to the philosopher structure.
 * @param status Status message of the philosopher.
 */
void	print_status(t_philo *p, const char *status)
{
	long	timestamp;

	timestamp = get_time() - p->env->start_time;
	buffered_print(&p->env->log_buffer, timestamp, p->id + 1, status);
}

/**
 * @brief Flushes log entries to standard output.
 *
 * This function prints all buffered log entries in a thread-safe manner.
 * After printing, the buffer is cleared for new entries.
 *
 * @param env Pointer to the environment structure.
 * @param log_count Number of log entries to flush.
 */
static void	flush_log_entries(t_env *env, int log_count)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&env->print_mutex);
	while (i < log_count)
	{
		printf("%ld %d %s\n",
			env->log_buffer.entries[i].timestamp,
			env->log_buffer.entries[i].id,
			env->log_buffer.entries[i].status);
		i++;
	}
	env->log_buffer.count = 0;
	pthread_mutex_unlock(&env->print_mutex);
}

/**
 * @brief Checks if the log flusher should exit.
 *
 * This function determines whether the log flushing thread should exit.
 * It checks if the simulation has ended and whether there are any remaining
 * log entries to be flushed.
 *
 * @param env Pointer to the environment structure.
 * @param log_count Pointer to store the number of log entries.
 * @return 1 if the log flusher should exit, otherwise 0.
 */
static int	should_exit_log_flusher(t_env *env, int *log_count)
{
	int	ended_local;

	pthread_mutex_lock(&env->end_mutex);
	ended_local = env->ended;
	pthread_mutex_unlock(&env->end_mutex);
	pthread_mutex_lock(&env->log_buffer.mutex);
	*log_count = env->log_buffer.count;
	if (ended_local && *log_count == 0)
	{
		pthread_mutex_unlock(&env->log_buffer.mutex);
		return (1);
	}
	return (0);
}

/**
 * @brief Thread function that continuously flushes log entries.
 *
 * This function runs in a separate thread and continuously flushes
 * the log buffer. It ensures logs are printed periodically and exits
 * when the simulation ends and all logs have been printed.
 *
 * @param arg Pointer to the environment structure (`t_env`).
 * @return NULL when the thread exits.
 */
void	*log_flusher(void *arg)
{
	t_env	*env;
	int		log_count;

	env = (t_env *)arg;
	while (1)
	{
		if (should_exit_log_flusher(env, &log_count))
			break ;
		flush_log_entries(env, log_count);
		pthread_mutex_unlock(&env->log_buffer.mutex);
		usleep(1000);
	}
	return (NULL);
}
