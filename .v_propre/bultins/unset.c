/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/02 03:14:48 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

void suppress_env(t_env *env, char *search)
{
	t_env	*current;
	t_env	*previous;

	current = env->next;
	previous = env;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, search) == 0)
		{
			iif (previous)
				previous->next = current->next;
			else if (current->next)
				env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			break;
		}
		previous = current;
		current = current->next;
	}
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
