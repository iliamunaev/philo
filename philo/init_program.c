/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_program.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:11:18 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 16:11:19 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_program(t_env **env, int ac, char **av)
{
	*env = malloc(sizeof(t_env));
	if (!*env)
	{
		print_error("Error: init_and_setup: env mem alloc failed\n");
		exit (EXIT_FAILURE);
	}
	if (init_env(*env, ac, av) == EXIT_FAILURE)
	{
		free_env(*env);
		exit (EXIT_FAILURE);
	}
}
