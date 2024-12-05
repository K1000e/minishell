#include "../include/minihell.h"

char	*ft_strndup(char *str, size_t len)
{
	char	*dup;
	size_t	len_dup;

	len_dup = ft_strlen(str);
	if (len_dup > len)
		len_dup = len;
	dup = ft_calloc((len_dup + 1), sizeof(char));
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, str, len_dup + 1);
	return (dup);
}

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	t_cmd *next;
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
		if (current->args_token)
		{
			i = -1;
			while (current->args_token[++i])
				free(current->args_token[i]);
			free(current->args_token);
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
}

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
	int	i;

	if (cmd_node)
	{
		if (cmd_node->cmd)
			free(cmd_node->cmd); // Free the command string
		if (cmd_node->args)
		{
			i = 0;
			while (cmd_node->args[i])
			{
				free(cmd_node->args[i]); // Free each argument
				i++;
			}
			free(cmd_node->args); // Free the args array
		}
		free(cmd_node); // Finally, free the command node itself
	}
}
