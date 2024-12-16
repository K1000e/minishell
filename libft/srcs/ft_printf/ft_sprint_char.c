/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprint_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 22:47:19 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/15 21:41:45 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_print_char_str(char *buffer, char c)
{
	ft_join(buffer, &c);
	return (1);
}

int	ft_print_s_str(char *buffer, char *s)
{
	if (s == NULL)
	{
		s = "(null)";
	}
	ft_join(buffer, s);
	return (ft_strlen(s));
}
