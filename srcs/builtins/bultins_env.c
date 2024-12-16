/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/15 22:13:07 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

t_env	*ft_find_key(t_env *env, char *key)
{
	t_env	*current;

	if (!env || !key)
		return (NULL);
	current = env->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (env);
}

void	ft_update_key(t_env *env, char *key, char *value)
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

void	ft_print_declare_env(t_env *env)
{
	t_env	*current;

	current = env->next;
	ft_sort_env(&current);
	while (current)
	{
		printf("declare -x %s", current->key);
		if (current->value)
			printf("=\"%s\"", current->value);
		printf("\n");
		current = current->next;
	}
}

t_bool	check_validity_export(const char *key)
{
	int	i;

	i = 0;
	if (!key || (!ft_isalpha(key[i]) && key[i] != '_'))
		return (FALSE);
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (FALSE);
	}
	return (TRUE);
}

void	ft_env(t_cmd *cmd, t_env *env)
{
	t_env	*current;

	if (cmd->args[1] != NULL)
	{
		printf("env: ‘%s’: No such file or directory\n", cmd->args[1]);
		return ;
	}
	current = env->next;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}
