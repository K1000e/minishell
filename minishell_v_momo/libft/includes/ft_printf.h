/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgorin <cgorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:10:50 by cgorin            #+#    #+#             */
/*   Updated: 2024/06/18 05:01:10 by cgorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>
# include <stdlib.h>
# include "libft.h"

//Fonctions UTILS

int		ft_putnbr_base(unsigned int n, char *base, int base_len);

//Fonctions PRINTF

int		ft_printf(const char *format, ...);
int		ft_print_char(char c);
int		ft_print_p(void *ptr);
int		ft_print_s(char *s);
int		ft_print_i_d(int number);
int		ft_print_x(unsigned int n, char format);
int		ft_print_u(unsigned int number);

#endif