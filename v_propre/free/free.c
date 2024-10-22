/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:32:09 by cgorin            #+#    #+#             */
/*   Updated: 2024/10/19 23:32:20 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

void free_cmd_list(t_cmd *cmd_list) {
	t_cmd *current;
	int i;

	while (cmd_list) {
		current = cmd_list;
		cmd_list = cmd_list->next;
		printf("Freeing command: %s\n", current->cmd); // Debug deallocation
		free(current->cmd);
		if (current->args) {
			i = -1;
			while (current->args[++i])
			{
				printf("Freeing argument[%d]: %s\n", i, current->args[i]);
				free(current->args[i]);
			}
			free(current->args);
		}
		free(current->out_file);
		free(current->in_file);
		free(current);
	}
}
