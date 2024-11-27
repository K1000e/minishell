#include "../include/minihell.h"

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
	return env;
}

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

    if (!ft_isalpha(key[i]) && key[i] != '_') // Must start with a letter or _
        return FALSE;
    while (key[++i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_') // Allow only alphanumerics or _
            return FALSE;
    }
    return TRUE;
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
			//printf("cmd->args[1][i] = %c\n", cmd->args[1][i]);
			if (cmd->args[1][i] == '=')
			{
				key = ft_strndup(cmd->args[1], i);
				if (!check_validity_export(key))
				{
					g_exit_code = 1;
					ft_fprintf(2, "1bash: %s: '%s': not a valid identifier\n", cmd->args[0], cmd->args[1]);
					return ;
				}
				value = ft_strdup(cmd->args[1] + i + 1);
				//printf("%s\n", cmd->args[1]);
				if (!key || !value || !key[0] || !value[0])
				{
					g_exit_code = 1;
					ft_fprintf(2, "2bash: %s: '%s': not a valid identifier\n", cmd->args[0], cmd->args[1]);
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
		if (!check_validity_export(cmd->args[1])) // ici args[1] est a 0 je comprends pas pourquoi !!!!
		{
			g_exit_code = 1;
			ft_fprintf(2, "3bash: %s: %s: not a valid identifier\n", cmd->args[0], cmd->args[1]);
			return ;
		}
		g_exit_code = 0;
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
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

void	ft_unset(t_cmd *cmd, t_env *env)
{
	t_env	*current;
	t_env	*previous;
	int		i;

	if (cmd->args[1] == NULL)
		return ;
	current = env->next;
	previous = env;
	i = 0;
	while (cmd->args[++i])
	{
		while (current)
		{
			if (current->key && ft_strcmp(current->key, cmd->args[i]) == 0)
			{
				if (previous)
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
		current = env;
	}
}
