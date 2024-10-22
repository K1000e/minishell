/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 18:33:23 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/19 22:42:22 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

t_bool ft_check_option_echo(t_cmd *cmd)
{
	int		i;
	t_bool	option;
	
	i = 0;
	option = FALSE;
	if (ft_strncmp(cmd->args[1], "-n", 2) == 0)
	{
		option = TRUE;
		while (cmd->args[1][++i])
		{
			if (cmd->args[1][i] != 'n')
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
	i = 0;
	if (cmd->args[1] != NULL)
	{
		option = ft_check_option_echo(cmd);
		i = option;
		while (cmd->args[++i])
		{
			printf("%s",cmd->args[i]);
			if (cmd->args[i + 1] != NULL)
				printf(" ");
		}
	}
	if (!option)
		printf("\n");
}
