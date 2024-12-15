#include "../include/minihell.h"

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
			ft_fprintf(2, "bash: export: '%s': not a valid identifier\n",
				cmd->args[i]);
			free(key);
			free(value);
			return ;
		}
		if (!env)
		{
			if (value)
				env = create_env_node(key, value);
			else
				env = create_env_node(key, "");
		}
		else
		{
			if (value)
				ft_update_key(env, key, value);
			else
				ft_update_key(env, key, "");
		}
		free(key);
		free(value);
	}
	g_exit_code = 0;
}
