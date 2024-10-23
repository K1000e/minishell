/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdessm <mabdessm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 22:14:03 by mabdessm          #+#    #+#             */
/*   Updated: 2024/10/23 04:23:00 by mabdessm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell_v_cam/include/minihell.h"

void ft_cd(t_cmd *cmd, t_env *env)
{
	char	*directory = NULL;
	t_env	*current;
	char	cwd[1024];

	if (cmd->args[2] != NULL)
	{
		printf("cd: too many arguments\n");
        return ;
	}
    if (cmd->args[1] == NULL)
    {
		current = ft_find_key(env, "HOME");
		if (current && current->value)
			directory = current->value + 1;
        else
        {
            printf("cd: HOME not set\n");
            return ;
        }
    }
    else
	{
        directory = cmd->args[1];
	}
								//error handling works
	if (chdir(directory) != 0) 	//memory issue when the directory is valid
	{
		perror("cd");
		return ;
	}
	if (env)
	{
		current = ft_find_key(env, "PWD");
		if (current && getcwd(cwd, sizeof(cwd)) != NULL)
		{
			ft_update_key(env, "OLDPWD", current->value);
			ft_update_key(env, "PWD", cwd);
		}
		else
		{
			perror("getcwd");
			return ;
		}
	}
}
