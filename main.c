/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael <wael@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 16:28:00 by wael              #+#    #+#             */
/*   Updated: 2026/02/01 00:40:23 by wael             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*eat(void *arg)
{
	printf("%s\n", (char *)arg);
	return (NULL);
}

int	main(int ac, char **av)
{
	pthread_t		thread;
	pthread_mutex_t	mutex;
	t_table		table;

	parse_args(ac, av, &table);
	
	return (EXIT_SUCCESS);
}
