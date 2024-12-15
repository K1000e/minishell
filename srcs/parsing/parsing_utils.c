/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:57:23 by mabdessm          #+#    #+#             */
/*   Updated: 2024/12/15 06:51:33 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minihell.h"

/* 
void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	t_cmd	*next;
	int		i;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		if (current->cmd)
			free(current->cmd);
		if (current->token)
			free(current->token);
		if (current->args)
		{
			i = -1;
			while (current->args[++i])
				free(current->args[i]);
			free(current->args);
		}
		if (current->args_t)
		{
			i = -1;
			while (current->args_t[++i])
				free(current->args_t[i]);
			free(current->args_t);
		}
		if (current->out_file)
		{
			i = -1;
			while (current->out_file[++i])
				free(current->out_file[i]);
			free(current->out_file);
		}
		if (current->in_file)
		{
			i = -1;
			while (current->in_file[++i])
				free(current->in_file[i]);
			free(current->in_file);
		}
		if (current->append)
			free(current->append);
		if (current->order_file)
			free(current->order_file);
		if (current->heredoc_delimiter)
		{
			i = -1;
			while (current->heredoc_delimiter[++i])
				free(current->heredoc_delimiter[i]);
			free(current->heredoc_delimiter);
		}
		free(current);
		current = next;
	}
} */

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

void	free_cmd_node(t_cmd *cmd_node)
{
	if (cmd_node->cmd)
		free(cmd_node->cmd);
	if (cmd_node->token)
		free(cmd_node->token);
	if (cmd_node->args)
		free_string_array(cmd_node->args);
	if (cmd_node->args_t)
		free_string_array(cmd_node->args_t);
	if (cmd_node->out_file)
		free_string_array(cmd_node->out_file);
	if (cmd_node->in_file)
		free_string_array(cmd_node->in_file);
	if (cmd_node->append)
		free(cmd_node->append);
	if (cmd_node->order_file)
		free(cmd_node->order_file);
	if (cmd_node->heredoc_delimiter)
		free_string_array(cmd_node->heredoc_delimiter);
	free(cmd_node);
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
	new_tab[i] = strdup(str);
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

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd_list;
	while (current)
	{
		next = current->next;
		free_cmd_node(current);
		current = next;
	}
}
