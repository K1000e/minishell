/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins_env_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/19 17:23:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	supress_env(t_env *env, char *search)
{
	t_env	*current;
	t_env	*previous;

	current = env->next;
	previous = env;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, search) == 0)
		{
			if (previous)
				previous->next = current->next;
			else if (current->next)
				env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			break ;
		}
		previous = current;
		current = current->next;
	}
	current = env;
}

void	ft_unset(t_cmd *cmd, t_env *env)
{
	int		i;

	if (cmd->args[1] == NULL)
		return ;
	i = 0;
	while (cmd->args[++i])
		supress_env(env, cmd->args[i]);
}

void	change_values(t_env *current, t_env *next)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = current->key;
	tmp_value = current->value;
	current->key = next->key;
	current->value = next->value;
	next->key = tmp_key;
	next->value = tmp_value;
}

void	ft_sort_env(t_env **env)
{
	t_env	*current;
	t_env	*next;
	int		swapped;

	if (!env || !(*env) || !(*env)->next)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = *env;
		while (current->next)
		{
			next = current->next;
			if (ft_strcmp(current->key, next->key) > 0)
			{
				change_values(current, next);
				swapped = 1;
			}
			current = current->next;
		}
	}
}
