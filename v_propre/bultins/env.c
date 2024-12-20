/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/19 22:43:31 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"


void	ft_env(t_cmd *cmd, t_env *env)
{
	t_env	*current;

	if (cmd->args[1] != NULL)
	{
		printf("env: ‘%s’: No such file or directory\n", cmd->args[1]);
		return ;
	}
	current = env;
	while (current)
	{
		printf("%s%s\n", current->key, current->value);
		current = current->next;
	}
}