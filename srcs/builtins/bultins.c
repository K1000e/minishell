/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultins.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/19 17:23:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_exit_code(int nb_args, t_cmd *cmd)
{
	int	exit_code;

	exit_code = 0;
	if (nb_args == 2)
		exit_code = ft_atoi(cmd->args[1]) % 256;
	printf("exit\n");
	return (exit_code);
}

void	ft_exit(t_cmd *cmd)
{
	int	nb_args;
	int	i;

	nb_args = 0;
	while (cmd->args[nb_args] != NULL)
		nb_args++;
	i = -1;
	if (cmd->args[1] && (cmd->args[1][0] == '-' || cmd->args[1][0] == '+'))
		i++;
	while (cmd->args[1] != NULL && cmd->args[1][++i])
	{
		if (!ft_isdigit(cmd->args[1][i]))
		{
			ft_fprintf(2, "exit: %s: numeric argument required\n",
				cmd->args[1]);
			exit(2);
		}
	}
	if (nb_args <= 2)
		exit(ft_exit_code(nb_args, cmd));
	ft_fprintf(2, "exit: too many arguments\n");
	g_exit_code = 1;
}

static t_bool	ft_check_option_echo(t_cmd *cmd, int j)
{
	int		i;
	t_bool	option;

	i = 0;
	option = FALSE;
	if (ft_strncmp(cmd->args[j], "-n", 2) == 0)
	{
		option = TRUE;
		while (cmd->args[j][++i])
		{
			if (cmd->args[j][i] != 'n')
				option = FALSE;
		}
	}
	return (option);
}

void	ft_echo(t_cmd *cmd)
{
	int		i;
	t_bool	option;

	option = FALSE;
	if (cmd->args[1])
	{
		option = ft_check_option_echo(cmd, 1);
		i = option;
		while (cmd->args[i + 1] && ft_check_option_echo(cmd, i + 1) == 1)
			i++;
		while (cmd->args[++i])
		{
			printf("%s", cmd->args[i]);
			if (cmd->args[i + 1] != NULL)
				printf(" ");
		}
	}
	if (!option)
		printf("\n");
	g_exit_code = 0;
}

void	ft_pwd(t_env *env)
{
	t_env	*current;
	char	*path;

	current = ft_find_key(env, "PWD");
	path = current->value;
	if (!path)
		return ;
	printf("%s\n", path);
	g_exit_code = 0;
}
