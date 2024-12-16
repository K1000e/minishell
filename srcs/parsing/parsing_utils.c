/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:57:23 by mabdessm          #+#    #+#             */
/*   Updated: 2024/12/16 20:23:56 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

void	ft_cmd_add_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*ptr;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ptr = *lst;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new;
}

t_bool	is_within_single_quotes(const char *input, int index)
{
	int		i;
	t_bool	single_quote;

	i = 0;
	single_quote = FALSE;
	while (i < index)
	{
		if (input[i] == '\'' && !single_quote)
			single_quote = TRUE;
		else if (input[i] == '\'' && single_quote)
			single_quote = FALSE;
		i++;
	}
	return (single_quote);
}

char	**add_to_tab(char **tab, const char *str)
{
	int		i;
	char	**new_tab;

	i = 0;
	if (tab)
		while (tab[i] != NULL)
			i++;
	new_tab = malloc((i + 2) * sizeof(char *));
	new_tab[i] = ft_strdup(str);
	if (!new_tab[i])
	{
		free(new_tab);
		return (NULL);
	}
	new_tab[i + 1] = NULL;
	while (--i >= 0)
		new_tab[i] = tab[i];
	if (tab)
		free(tab);
	return (new_tab);
}

t_bool	return_error(char *error, int exit_code)
{
	ft_fprintf(2, "%s\n", error);
	g_exit_code = exit_code;
	return (FALSE);
}
