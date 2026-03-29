/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael <wael@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 16:28:00 by wael              #+#    #+#             */
/*   Updated: 2026/03/29 00:00:00 by wael             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(philo->table->dead_lock);
	if (!philo->table->is_dead)
	{
		pthread_mutex_lock(philo->table->print);
		printf("%zu %zu %s\n", get_time() - philo->table->start_time, philo->id, status);
		pthread_mutex_unlock(philo->table->print);
	}
	pthread_mutex_unlock(philo->table->dead_lock);
}

void	cleanup(t_table *table)
{
	size_t	i;

	i = 0;
	while (i < table->philos_count)
	{
		pthread_mutex_destroy(table->forks[i]);
		free(table->forks[i]);
		pthread_mutex_destroy(table->philos[i].meal_lock);
		free(table->philos[i].meal_lock);
		i++;
	}
	free(table->forks);
	pthread_mutex_destroy(table->print);
	free(table->print);
	pthread_mutex_destroy(table->dead_lock);
	free(table->dead_lock);
	free(table->philos);
}

void	init_table(t_table *table)
{
	size_t	i;

	table->is_dead = 0;
	table->start_time = get_time();
	table->print = malloc(sizeof(pthread_mutex_t));
	if (!table->print)
		exit(EXIT_FAILURE);
	pthread_mutex_init(table->print, NULL);
	table->dead_lock = malloc(sizeof(pthread_mutex_t));
	if (!table->dead_lock)
		exit(EXIT_FAILURE);
	pthread_mutex_init(table->dead_lock, NULL);
	table->philos = malloc(sizeof(t_philo) * table->philos_count);
	if (!table->philos)
		exit(EXIT_FAILURE);
	table->forks = malloc(sizeof(pthread_mutex_t *) * table->philos_count);
	if (!table->forks)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < table->philos_count)
	{
		table->forks[i] = malloc(sizeof(pthread_mutex_t));
		if (!table->forks[i])
			exit(EXIT_FAILURE);
		pthread_mutex_init(table->forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < table->philos_count)
	{
		table->philos[i].id = i + 1;
		table->philos[i].last_eat = get_time();
		table->philos[i].meal_count = 0;
		table->philos[i].table = table;
		table->philos[i].meal_lock = malloc(sizeof(pthread_mutex_t));
		if (!table->philos[i].meal_lock)
			exit(EXIT_FAILURE);
		pthread_mutex_init(table->philos[i].meal_lock, NULL);
		table->philos[i].left_fork = table->forks[i];
		table->philos[i].right_fork = table->forks[(i + 1) % table->philos_count];
		i++;
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (1)
	{
		pthread_mutex_lock(philo->table->dead_lock);
		if (philo->table->is_dead)
		{
			pthread_mutex_unlock(philo->table->dead_lock);
			break ;
		}
		pthread_mutex_unlock(philo->table->dead_lock);
		
		// Eating
		if (philo->id % 2 != 0)
		{
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
			if (philo->table->philos_count == 1)
			{
				ft_usleep(philo->table->time_to_die);
				pthread_mutex_unlock(philo->left_fork);
				break ;
			}
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
		}
		print_status(philo, "is eating");
		pthread_mutex_lock(philo->meal_lock);
		philo->last_eat = get_time();
		philo->meal_count++;
		pthread_mutex_unlock(philo->meal_lock);
		ft_usleep(philo->table->time_to_eat);
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);

		// Sleeping
		print_status(philo, "is sleeping");
		ft_usleep(philo->table->time_to_sleep);

		// Thinking
		print_status(philo, "is thinking");
	}
	return (NULL);
}

void	monitor_philos(t_table *table)
{
	size_t	i;
	int		all_ate;

	while (1)
	{
		i = 0;
		all_ate = 1;
		while (i < table->philos_count)
		{
			pthread_mutex_lock(table->philos[i].meal_lock);
			if (get_time() - table->philos[i].last_eat > table->time_to_die)
			{
				pthread_mutex_lock(table->dead_lock);
				table->is_dead = 1;
				pthread_mutex_unlock(table->dead_lock);
				pthread_mutex_lock(table->print);
				printf("%zu %zu died\n", get_time() - table->start_time, table->philos[i].id);
				pthread_mutex_unlock(table->print);
				pthread_mutex_unlock(table->philos[i].meal_lock);
				return ;
			}
			if (table->max_meals != -1 && table->philos[i].meal_count < (size_t)table->max_meals)
				all_ate = 0;
			pthread_mutex_unlock(table->philos[i].meal_lock);
			i++;
		}
		if (table->max_meals != -1 && all_ate)
		{
			pthread_mutex_lock(table->dead_lock);
			table->is_dead = 1;
			pthread_mutex_unlock(table->dead_lock);
			return ;
		}
		usleep(1000);
	}
}

int	main(int ac, char **av)
{
	t_table	table;
	size_t	i;

	parse_args(ac, av, &table);
	init_table(&table);
	i = 0;
	while (i < table.philos_count)
	{
		if (pthread_create(&table.philos[i].thread, NULL, philo_routine, &table.philos[i]) != 0)
			exit(EXIT_FAILURE);
		i++;
	}
	monitor_philos(&table);
	i = 0;
	while (i < table.philos_count)
	{
		pthread_join(table.philos[i].thread, NULL);
		i++;
	}
	cleanup(&table);
	return (EXIT_SUCCESS);
}
