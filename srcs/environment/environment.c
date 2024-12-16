/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:57:11 by mabdessm          #+#    #+#             */
/*   Updated: 2024/12/16 13:08:59 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

t_env	*create_env_node(char *key, char *value)
{
	t_env	*new_env;

	new_env = NULL;
	new_env = malloc(sizeof(t_env));
	if (!new_env)
	{
		return (NULL);
	}
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	new_env->next = NULL;
	return (new_env);
}

char	**base_env(t_env *env)
{
	t_env	*current;
	char	**environment;
	int		i;

	current = env->next;
	environment = ft_calloc(ft_envsize(env) + 1, sizeof(char *));
	i = 0;
	while (current)
	{
		environment[i] = ft_strjoin(current->key, "=");
		environment[i] = ft_strjoin_free(environment[i], current->value, 1);
		i++;
		current = current->next;
	}
	return (environment);
}

t_env	*intialize_env(t_env *new_env, char *executable)
{
	char	*exe;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	new_env->key = NULL;
	new_env->value = NULL;
	new_env->next = NULL;
	exe = ft_strrchr(executable, '/');
	if (exe)
		new_env->executable = ft_strdup(exe + 1);
	else
		new_env->executable = ft_strdup(executable);
	return (new_env);
}

t_env	*create_env(char **env, t_env *new_env, char *executable, int i)
{
	int		j;
	t_env	*env_node;
	char	*value;
	char	*key;

	new_env = intialize_env(new_env, executable);
	while (env[++i])
	{
		j = -1;
		while (env[i][++j])
		{
			if (env[i][j] == '=')
			{
				key = ft_strndup(env[i], j);
				value = ft_strdup(env[i] + j + 1);
				env_node = create_env_node(key, value);
				ft_env_add_back_(&new_env, env_node);
				free(key);
				free(value);
				break ;
			}
		}
	}
	return (new_env);
}
