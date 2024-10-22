/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/19 22:42:52 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

void	ft_unset(t_cmd *cmd, t_env *env)
{
	t_env	*current;
	t_env	*previous;

	if (cmd->args[1] == NULL)
		return ;
	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, cmd->args[1]) == 0)
		{
			if (previous)
				previous->next = current->next;
			else
				env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}
