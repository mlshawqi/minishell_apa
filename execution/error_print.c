/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machaouk <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-21 09:57:34 by machaouk          #+#    #+#             */
/*   Updated: 2025-05-21 09:57:34 by machaouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	malloc_error(const char *context)
{
	if (context)
	{
		write(2, "warning: failed to allocate ", 28);
		write(2, context, strlen(context));
		write(2, "\n", 1);
	}
	else
		write(2, "warning: malloc failed\n", 23);
	g_last_exit_code = 1;
	return (1);
}

void	print_cmd_error(const char *cmd, const char *msg, char *option)
{
	write(2, cmd, strlen(cmd));
	write(2, ": ", 2);
	if (option)
	{
		write(2, option, strlen(option));
		write(2, ": ", 2);
	}
	write(2, msg, strlen(msg));
	write(2, "\n", 1);
}
