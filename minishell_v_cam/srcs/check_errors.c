#include "minihell.h"

t_bool count_redir(const char *line)
{
	int j;

	j = -1;
	while (line[++j])
	{
		if (line[j] == '>')
		{
			if (line[j + 1] == '>')
			{
				if (line[j + 2] == '>')
					return FALSE;
				j++;
			}
			j++;
		}
		if (line[j] == '<')
		{
			if (line[j + 1] == '<')
			{
				if (line[j + 2] == '<')
					return FALSE;
				j++;
			}
			j++;
		}
	}
	return TRUE;
}

t_bool match_quotes(char *line)
{
	int i;
	t_bool d_q;
	t_bool s_q;

	i = -1;
	d_q = FALSE;
	s_q = FALSE;
	while (line[++i])
	{
		if (line[i] == '"' && d_q == FALSE && s_q == FALSE)
			d_q = TRUE;
		else if (line[i] == '"' && d_q == TRUE && s_q == FALSE)
		   d_q = FALSE;
		else if (line[i] == '\'' && d_q == FALSE && s_q == FALSE)
			s_q = TRUE;
		else if (line[i] == '\'' && d_q == FALSE && s_q == TRUE)
		   s_q = FALSE;
	}
	if (d_q == TRUE || s_q == TRUE)
	{
		printf("Error: Unmatched quote\n");
		return FALSE;
	}

	return TRUE;
}
