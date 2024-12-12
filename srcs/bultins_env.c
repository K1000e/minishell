#include "../include/minihell.h"

t_env	*ft_find_key(t_env *env, char *key)
{
	t_env	*current;

	if (!env || !key)
		return NULL;
	current = env->next;

	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return env;
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

void	ft_sort_env(t_env **env)
{
	t_env   *current;
	t_env   *next;
	char	*tmp_key;
	char	*tmp_value;
	int	 swapped;

	if (!env || !(*env) || !(*env)->next)
		return;

	do {
		swapped = 0;
		current = *env;
		while (current->next)
		{
			next = current->next;
			if (strcmp(current->key, next->key) > 0)
			{
				tmp_key = current->key;
				tmp_value = current->value;
				current->key = next->key;
				current->value = next->value;
				next->key = tmp_key;
				next->value = tmp_value;
				swapped = 1;
			}
			current = current->next;
		}
	} while (swapped);
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

void	ft_export(t_cmd *cmd, t_env *env)
{
	char	*value;
	char	*key;
	int		i;
	char	*equal_sign;

	if (!cmd->args[1])
	{
		ft_print_declare_env(env);
		return ;
	}
	i = 0;
	while (cmd->args[++i])
	{
		equal_sign = ft_strchr(cmd->args[i], '=');
		if (equal_sign)
		{
			key = ft_strndup(cmd->args[i], equal_sign - cmd->args[i]);
			value = ft_strdup(equal_sign + 1);
			if (!value)
				value = ft_strdup("");
		}
		else
		{
			key = ft_strdup(cmd->args[i]);
			value = NULL;
		}

		if (!check_validity_export(key) || !key[0])
		{
			g_exit_code = 1;
			ft_fprintf(2, "bash: export: '%s': not a valid identifier\n", cmd->args[i]);
			free(key);
			free(value);
			return;
		}
		if (!env)
			env = create_env_node(key, value ? value : "");
		else
			ft_update_key(env, key, value ? value : "");
		free(key);
		free(value);
	}
	g_exit_code = 0;
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
