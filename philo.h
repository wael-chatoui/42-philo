/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael <wael@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 16:46:46 by wael              #+#    #+#             */
/*   Updated: 2026/02/01 00:39:55 by wael             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>

typedef struct s_table	t_table;

typedef struct s_philo
{
	size_t			id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	size_t			last_eat;
	pthread_mutex_t	*meal_lock;
	t_table			*table;
	size_t			meal_count;
}	t_philo;

typedef struct s_table
{
	t_philo			*philos;
	size_t			philos_count;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				max_meals;
	size_t			start_time;
	int				is_dead;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	**forks;
	pthread_mutex_t	*print;
}	t_table;

void	parse_args(int ac, char **av, t_table *table);
size_t	get_time(void);
void	ft_usleep(size_t time);

#endif