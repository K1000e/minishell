/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:56:01 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/16 13:23:29 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

static char	*handle_env_variable(const char *input, int *i, t_env *env)
{
	char	*var_name;
	char	*value;
	int		start;
	int		len;

	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	len = *i - start;
	var_name = ft_substr(input, start, len);
	if (!var_name)
		return (NULL);
	value = ft_find_key(env, var_name)->value;
	free(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*handle_special_cases(const char *input, int *i, t_env *env)
{
	if (input[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(g_exit_code));
	}
	else if (input[*i] == '0')
	{
		(*i)++;
		return (ft_strdup(env->executable));
	}
	else if (ft_isdigit(input[*i]))
	{
		(*i)++;
		return (NULL);
	}
	return (NULL);
}

static char	*append_char(char *result, char c)
{
	char	temp[2];
	char	*new_result;

	temp[0] = c;
	temp[1] = '\0';
	new_result = ft_strjoin(result, temp);
	free(result);
	return (new_result);
}

static char	*process_input(const char *input, t_env *env, int i)
{
	char	*result;
	char	*to_append;

	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] && (input[i + 1] == '?'
				|| ft_isalpha(input[i + 1]) || input[i + 1] == '_'
				|| ft_isdigit(input[i + 1])))
		{
			if (is_within_single_quotes(input, i) || input[i + 1] == '"')
			{
				result = append_char(result, input[i++]);
				continue ;
			}
			if (ft_isalpha(input[++i]) || input[i] == '_')
				to_append = handle_env_variable(input, &i, env);
			else
				to_append = handle_special_cases(input, &i, env);
		}
		else
			to_append = append_char(ft_strdup(""), input[i++]);
		result = ft_strjoin_free(result, to_append, 3);
	}
	return (result);
}

char	*expand_env_vars(char *input, t_env *env)
{
	if (!input)
		return (NULL);
	input = process_input(input, env, 0);
	return (input);
}
