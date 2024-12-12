/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 03:21:16 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/02 03:22:16 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

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
	value = get_env_var_value(var_name, env);
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

t_bool is_within_single_quotes(const char *input, int index)
{
	int i = 0;
	t_bool single_quote = FALSE;

	while (i < index)
	{
		if (input[i] == '\'' && !single_quote)
			single_quote = TRUE;
		else if (input[i] == '\'' && single_quote)
			single_quote = FALSE;
		i++;
	}
	return single_quote;
}

static char	*process_input(const char *input, t_env *env)
{
	char	*result;
	char	*to_append;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] && (input[i + 1] == '?' || ft_isalpha(input[i + 1]) || input[i + 1] == '_'))
		{
			if (is_within_single_quotes(input, i) || input[i + 1] == '"')
			{
				result = append_char(result, input[i++]);
				continue;
			}
			i++;
			if (ft_isalpha(input[i]) || input[i] == '_')
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
	input = process_input(input, env);
	return (input);
}
