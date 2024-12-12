/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/02 03:10:44 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

void ft_update_key(t_env *env, char *key, char *value)
{
	t_env	*env_node;
	t_env	*current;

	current = ft_find_key(env, key);
	if (current->key != NULL)
	{
		free(current->value);
		current->value = ft_strdup(value);
	}
	else
	{
		if (!env)
			env = create_env_node(key, value);
		else
		{
			env_node = create_env_node(key, value);
			ft_env_add_back_(&env, env_node);
		}
	}
}

void ft_print_declare_env(t_env *env)
{
	t_env *current;

	current = env->next;
	while (current)
	{
		printf("declare -x %s=%s\n", current->key, current->value);
		current = current->next;
	}
}

t_bool check_validity_export(char *key)
{
	int i = 0;

	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (FALSE);
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (FALSE);
	}
	return (TRUE);
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
				value = ft_strdup(cmd->args[1] + i + 1);
				if (!check_validity_export(key) || !key || !value 
					|| !key[0] || !value[0])
				{
					g_exit_code = 1;
					ft_fprintf(2, "bash: %s: '%s': not a valid identifier\n", cmd->args[0], cmd->args[1]);
					return ;
				}
				else if (!env)
					env = create_env_node(key, value);
				else
					ft_update_key(env, key, value);
				free(key);
				free(value);
				g_exit_code = 0;
				return ;
			}

		}
		if (!check_validity_export(cmd->args[1]))
		{
			g_exit_code = 1;
			ft_fprintf(2, "bash: %s: %s: not a valid identifier\n", cmd->args[0], cmd->args[1]);
			return ;
		}
		g_exit_code = 0;
	}
}