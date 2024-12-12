/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   momo$.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdessm <mabdessm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 21:19:03 by mabdessm          #+#    #+#             */
/*   Updated: 2024/11/25 09:55:24 by mabdessm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// get_env_var_value: This function takes a variable name and looks it up in env.
// If it finds a match, it returns the value; otherwise, it returns NULL.

// expand_env_vars: This function walks through the command string (input),
// copying characters to result. When it encounters a $ followed by a variable name,
// it replaces it with the variable's value from the environment or leaves it blank
// if the variable isn't defined.

#include "../include/minihell.h"

t_env	*ft_find_key(t_env *env, char *key)
{
	t_env	*current;

	current = env->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

char	*get_env_var_value(const char *var_name, t_env *env)
{
	t_env	*current;

	current = env->next;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

char	*expand_env_vars(const char *input, t_env *env)
{
	char		*result;
	char		*res_ptr;
	const char	*ptr;
	int			i;
	char		*value;
	char 		*var_name;

	result = malloc(sizeof(char) * (ft_strlen(input) + 1));
	res_ptr = result;
	ptr = input;
	var_name = ft_calloc(sizeof(char), ft_strlen(input) + 1);
	while (*ptr)
	{
		if (*ptr == '$' && (ft_isalpha(ptr[1]) || ptr[1] == '_'))
		{
			i = 0;
			while (ft_isalnum(*ptr) || *ptr == '_')
			{
				var_name[i] = *ptr;
				++i;
				++ptr;
			}
			var_name[i] = '\0';
			value = get_env_var_value(var_name, env);
			if (value)
			{
				ft_strcpy(res_ptr, value);
				res_ptr += strlen(value);
			}
			++ptr;
		}
		else
		{
			*res_ptr = *ptr;
			++res_ptr;
			++ptr;
		}
	}
	*res_ptr = '\0';
	free(var_name);
	return (result);
}

// the ft_command in main.c changed a little bit
void	ft_command(char *line, t_env *env)
{
	char	*expanded_line;
	t_cmd	*commands;
	t_cmd	*tmp;

	// Expand env variables in the command line
	expanded_line = expand_env_vars(line, env);
	commands = parse_command(expanded_line);
	free(expanded_line);
	if (!commands || commands->cmd[0] == '\0')
		return ;
	is_pipe(commands);
	print_cmd_list(commands);
	tmp = commands;
	execute_command(tmp, env);
	free_cmd_list(commands);
}
