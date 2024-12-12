#include "../include/minihell.h"

void	print_cmd_list(t_cmd *cmd_lst)
{
	t_cmd	*current;
	int		index;
	int		i;

	current = cmd_lst;
	index = 0;
	while (current)
	{
		printf("Command Node %d\n", index);
		if (current->cmd)
			printf("  Command: %s\n", current->cmd);
		if (current->args)
		{
			i = 0;
			while (current->args[i])
			{
				printf("  Arguments[%d]: ", i);
				printf("%s \n", current->args[i]);
				i++;
			}
		}
		printf(" Redirection: %d\n", current->redirection);
		if (current->nb_infile)
		{
			printf("infile -> %d\n", current->nb_infile);
			i = -1;
			while (current->in_file[++i])
				printf("  Input Redirection: %s\n", current->in_file[i]);
		}
		if (current->nb_outfile)
		{
			printf("outfile %d\n", current->nb_outfile);
			i = -1;
			while (current->out_file[++i] && i < current->nb_outfile)
			{
				printf("  Output Redirection: %s\n", current->out_file[i]);
				printf("  Output Append ?%d\n", current->append[i]);
			}
		}
		current = current->next;
		index++;
	}
	printf("complete command print \n");
}
