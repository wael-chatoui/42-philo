/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael <wael@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 00:31:31 by wael              #+#    #+#             */
/*   Updated: 2026/02/01 00:32:05 by wael             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atoi(const char *str)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + *str - '0';
		str++;
	}
	return (res * sign);
}

void	parse_args(int ac, char **av, t_table *table)
{
	int	pc;
	int	ttd;
	int	tte;
	int	tts;

	if (ac != 5 && ac != 6)
		exit(EXIT_FAILURE);
	pc = ft_atoi(av[1]);
	ttd = ft_atoi(av[2]);
	tte = ft_atoi(av[3]);
	tts = ft_atoi(av[4]);
	if (pc <= 0 || ttd <= 0 || tte <= 0 || tts <= 0)
		exit(EXIT_FAILURE);
	table->philos_count = (size_t)pc;
	table->time_to_die = (size_t)ttd;
	table->time_to_eat = (size_t)tte;
	table->time_to_sleep = (size_t)tts;
	if (ac == 6)
	{
		table->max_meals = ft_atoi(av[5]);
		if (table->max_meals <= 0)
			exit(EXIT_FAILURE);
	}
	else
		table->max_meals = -1;
}