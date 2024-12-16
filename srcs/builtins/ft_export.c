/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 06:12:10 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

static void	handle_export_error(char *key, char *arg)
{
	g_exit_code = 1;
	ft_fprintf(2, "bash: export: '%s': not a valid identifier\n", arg);
	free(key);
}

static void	process_export_arg(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key = ft_strndup(arg, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
		if (!value)
			value = ft_strdup("");
	}
	else
	{
		key = ft_strdup(arg);
		value = NULL;
	}
	if (!check_validity_export(key) || !key[0])
		return (handle_export_error(key, arg));
	if (value)
		ft_update_key(*env, key, value);
	else
		ft_update_key(*env, key, "");
	free(key);
	free(value);
}

void	ft_export(t_cmd *cmd, t_env *env)
{
	int	i;

	if (!cmd->args[1])
		return (ft_print_declare_env(env));
	i = 0;
	while (cmd->args[++i])
		process_export_arg(cmd->args[i], &env);
}
