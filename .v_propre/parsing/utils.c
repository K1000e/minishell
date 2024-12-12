/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:31:25 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/19 23:32:36 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

char	*ft_strndup(char *str, size_t len)
{
	char	*dup;
	size_t	len_dup;

	len_dup = ft_strlen(str);
	if (len_dup > len)
		len_dup = len;
	dup = malloc((len_dup + 1) * sizeof(char));
	if (!dup)
		return NULL;
	ft_strlcpy(dup, str, len_dup + 1);
	dup[len_dup] = '\0';
	return (dup);
}

t_cmd *create_cmd_node(char *cmd_str, char *cmd_tokens)
{
	t_cmd *new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return NULL;
	new_cmd->cmd = ft_strdup(cmd_str);
	new_cmd->args = tokenise_command(cmd_str, cmd_tokens);
	new_cmd->out_file = NULL;
	new_cmd->in_file = NULL;
	new_cmd->next = NULL;
	new_cmd->append = FALSE;
	redirection(new_cmd);
	return (new_cmd);
}
