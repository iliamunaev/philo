/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 17:44:58 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 18:00:20 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file print_error.c
 * @brief Error handling function for philosopher simulation.
 */

#include "philo.h"

/**
 * @brief Prints an error message to standard output.
 *
 * This function writes the given error message to STDOUT_FILENO.
 *
 * @param msg Pointer to the error message string.
 */
void	print_error(char *msg)
{
	size_t	len_msg;

	len_msg = ft_strlen(msg);
	(void)write(STDOUT_FILENO, msg, len_msg);
}
