
t_list	*create_cmd_list(char *all, char *tmp, int i, int j)
{
	char	*cmd_str;
	char	*cmd_tokens;
	char	**cmd_split;
	t_list	*list_cmd;

	cmd_str = ft_strndup(&all[i], j - i);
	cmd_tokens = ft_strndup(&tmp[i], j - i);
	cmd_split = tokenise_command(cmd_str, cmd_tokens);
	list_cmd = ft_lstnew(cmd_split);
	free(cmd_str);
	free(cmd_tokens);
	return (list_cmd);
}

t_list	*parse_cmd(char *all)
{
	t_list	*list_cmd;
	char	*tmp;
	int		i;
	int		j;

	list_cmd = NULL;
	tmp = ft_strdup(all);
	i = -1;
	while (tmp[++i])
		tmp[i] = check_all_char(tmp[i]);
	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == 's' && ++i)
			continue;
		j = i;
		while (tmp[j] && tmp[j] != 's')
			j++;
		ft_lstadd_back(&list_cmd, create_cmd_list(all, tmp, i, j));
		i = j;
	}
	free(tmp);
	return (list_cmd);
}