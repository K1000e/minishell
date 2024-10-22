/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/19 22:44:00 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

void ft_update_key(t_env *env, char *key, char *value)
{
	t_env	*env_node;
	t_env	*current;

	current = env;
	while(current)
	{
		if (ft_strcmp(current->key, key) == 0)
			break;
		current = current->next;
	}
	if (ft_strcmp(current->key, key) == 0)
	{
		free(current->value);
		current->value = value;
	}
	else
	{
		env_node = create_env_node(key, value);
		ft_env_add_back_(&env, env_node);
	}
	free(key);
	free(value);
}

void ft_print_declare_env(t_env *env)
{
	t_env *current;

	current = env;
	while (current)
	{
		printf("declare -x %s%s\n", current->key, current->value);
		current = current->next;
	}
}

void	ft_export(t_cmd *cmd, t_env *env)
{
	char	*value;
	char	*key;
	int		i = -1;

	if (cmd->args[1] == NULL)
		ft_print_declare_env(env);
	else 
	{
		while (cmd->args[1][++i])
		{
			if (cmd->args[1][i] == '=')
			{
				key = ft_strndup(cmd->args[1], i);
				value = ft_strdup(cmd->args[1] + i);
				ft_update_key(env, key, value);
				return ;
			}
		}
	}
}