/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprint_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 22:47:19 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/12 16:35:50 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_char_fd(int fd, char c)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_print_s_fd(int fd, char *s)
{
	if (s == NULL)
	{
		s = "(null)";
	}
	write(fd, s, ft_strlen(s));
	return (ft_strlen(s));
}
