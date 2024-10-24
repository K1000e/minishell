/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdessm <mabdessm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 22:14:03 by mabdessm          #+#    #+#             */
/*   Updated: 2024/10/24 10:46:14 by mabdessm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell_v_cam/include/minihell.h"

void ft_update_key(t_env *env, char *key, char *value)
{
	t_env	*env_node;
	t_env	*current;

	current = env;
	while(current)
	{
		//printf("TEST 1\n");
		if (ft_strcmp(current->key, key) == 0)
			break;
		current = current->next;
	}
	if (ft_strcmp(current->key, key) == 0)
	{
		//printf("TEST 2\n");
		free(current->value);
		current->value = value;
	}
	else
	{
		//printf("TEST 3\n");
		env_node = create_env_node(key, value);
		ft_env_add_back_(&env, env_node);
	}
	//printf("TEST 4\n"); //does print
	//free(key);
	//free(value);
	//printf("TEST 5\n"); //DOES NOT PRINT BECAUSE OF FREES
}

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
	if (chdir(directory) != 0)
	{
		perror("cd");
		return ;
	}
	if (env)
	{
		current = ft_find_key(env, "PWD");
		if (current && getcwd(cwd, sizeof(cwd)) != NULL)
		{
			//printf("working\n"); //is printed
			ft_update_key(env, "OLDPWD", current->value);
			//printf("working\n"); //IS NOT PRINTED
			ft_update_key(env, "PWD", cwd);
		}
		else
		{
			perror("getcwd");
			return ;
		}
	}
}

/*void ft_cd(t_cmd *cmd, t_env *env)
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
			directory = ft_strdup(current->value + 1);
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
		free(directory);
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		free(directory);
		return;
	}
	if (env)
	{
		current = ft_find_key(env, "PWD");
		if (current)
		{
			ft_update_key(env, "OLDPWD", ft_strdup(current->value));
			ft_update_key(env, "PWD", ft_strdup(cwd));
		}
	}
	free(directory);
}
*/