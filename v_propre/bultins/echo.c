/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/02 03:09:31 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

t_bool ft_check_option_echo(t_cmd *cmd, int j)
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
			printf("%s",cmd->args[i]);
			if (cmd->args[i + 1] != NULL)
				printf(" ");
		}
	}
	if (!option)
		printf("\n");
	g_exit_code = 0;
}