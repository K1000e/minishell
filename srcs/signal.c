/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:31:25 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/12 16:08:09 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minihell.h"

void	sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sigint_heredoc_handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		close(STDIN_FILENO);
	}
}

void	set_signal_action(void (*handler)(int))
{
	struct sigaction	act;

	ft_bzero(&act, sizeof(act));
	act.sa_handler = handler;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
}
