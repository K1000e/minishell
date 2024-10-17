#include "minihell.h"
void	ft_env(t_cmd *cmd, t_env *env)
{
	t_env	*current;

	if (cmd->args[1] != NULL)
	{
		printf("env: ‘%s’: No such file or directory\n", cmd->args[1]);
		return ;
	}
	current = env;
	while (current)
	{
		printf("%s%s\n", current->key, current->value);
		current = current->next;
	}
}

void	ft_unset(t_cmd *cmd, t_env *env)
{
	t_env    *current;
    t_env    *previous;

    if (cmd->args[1] == NULL)
        return ;
    current = env;
    //previous = NULL;
    while (current)
    {
        if (ft_strcmp(current->key, cmd->args[1]) == 0)
        {
            if (previous)
                previous->next = current->next;
            else
                env = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return ;
        }
        previous = current;
        current = current->next;
    }
}

void    ft_export(t_cmd *cmd, t_env *env)
{
	(void)cmd;
	(void)env;
}