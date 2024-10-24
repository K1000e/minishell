#include "minihell.h"

t_env *ft_find_key(t_env *env, char *key)
{
	t_env *current;

	current = env->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return NULL;
}

void ft_update_key(t_env *env, char *key, char *value)
{
	t_env	*env_node;
	t_env	*current;

	current = ft_find_key(env, key);
	if (current)
	{
		free(current->value);
		current->value = ft_strdup(value);
	}
	else
	{
		env_node = create_env_node(key, value);
		ft_env_add_back_(&env, env_node);
	}
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
				free(key);
				free(value);
				return ;
			}
		}
	}
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
		printf("%s%s\n", current->key, current->value);
		current = current->next;
	}
}

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
