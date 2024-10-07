#include "minihell.h"

int main () 
{
	while (1) 
	{
		char *input = readline( "MINIHELL> " );  
		if (!input) 
			break ;
		add_history(input);

		// test du parsing
		/* t_list *commands = parse_cmd(input);
		t_list *tmp = commands;
		int cmd_index = 0;
		while (tmp)
		{
			printf("Commande %d :\n", cmd_index);
			char **cmd_tokens = (char **)tmp->content;
			for (int i = 0; cmd_tokens[i] != NULL; i++)
				printf("  Token %d: %s\n", i, cmd_tokens[i]);
			tmp = tmp->next;
			cmd_index++;
		}
		tmp = commands;
		while (tmp)
		{
			char **cmd_tokens = (char **)tmp->content;
			for (int i = 0; cmd_tokens[i] != NULL; i++) 
				free(cmd_tokens[i]);
			free(cmd_tokens);
			t_list *next = tmp->next;
			free(tmp);
			tmp = next;
		} */
		free (input); 
	} 
	return 0 ;
}