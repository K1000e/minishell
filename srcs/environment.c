#include "minihell.h"

t_env *create_env_node(char *key, char *value)
{
	t_env *new_env;

	new_env = NULL;
	new_env = malloc(sizeof(t_env));
	if (!new_env) {
		return NULL;
	}
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	new_env->next = NULL;
	return (new_env);
}

t_env	*ft_lstlast_env(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_env_add_back_(t_env **lst, t_env *new)
{
	t_env	*ptr;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr = ft_lstlast_env(*lst);
	ptr->next = new;
}


int	ft_envsize(t_env *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

char **base_env(t_env *env)
{
	t_env *current;
	char **environment;
	int i;

	current = env;
	environment = ft_calloc(ft_envsize(env) + 1, sizeof(char *));
	i = 0;
	while (current)
	{
		environment[i++] = ft_strjoin(env->key, env->value);
		current = current->next;
	}
	return (environment);
}

t_env	*get_env(char **env, t_env *new_env, char *executable)
{
	int		i;
	int		j;
	t_env	*env_node;
	char	*value;
	char	*key;

	i = -1;
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return NULL;
	new_env->key = NULL;
	new_env->value = NULL;
	new_env->next = NULL;
	new_env->executable = executable;
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
