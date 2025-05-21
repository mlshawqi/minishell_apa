/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: machaouk <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-21 09:59:05 by machaouk          #+#    #+#             */
/*   Updated: 2025-05-21 09:59:05 by machaouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_len(char *str1, char *str2, char hint)
{
	int	len1;
	int	len2;

	len1 = ft_strlen(str1);
	len2 = 0;
	while (str2[len2] != hint)
		len2++;
	if (len1 > len2)
		return (len1);
	else
		return (len2);
}

static int	check_overflow(int sign, unsigned long long nbr)
{
	if (sign < 0)
	{
		if (nbr > MIN_LONG)
		{
			print_cmd_error("minishell: exit", "numeric argument required",
				NULL);
			return (-1);
		}
	}
	else
	{
		if (nbr > MAX_LONG)
		{
			print_cmd_error("minishell: exit", "numeric argument required",
				NULL);
			return (-1);
		}
	}
	return (0);
}

long long	ft_atoii(const char *str)
{
	unsigned long long	result;
	int					sign;
	int					i;

	result = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result *= 10;
		result += str[i] - 48;
		if (check_overflow(sign, result) == -1)
			return (2);
		i++;
	}
	return (result * sign);
}
