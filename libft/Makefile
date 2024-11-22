NAME		:=	libft.a

CC			:=	cc
FLAGS		:= -Wall -Werror -Wextra

DIR_SRCS	:=	srcs
DIR_OBJS	:=	.objs
DIR_INCS	:=	includes


LST_SRCS	:= 	libft/ft_atoi.c			\
				libft/ft_bzero.c		\
				libft/ft_calloc.c		\
				libft/ft_isalnum.c		\
				libft/ft_isalpha.c		\
				libft/ft_isascii.c		\
				libft/ft_isdigit.c		\
				libft/ft_isprint.c		\
				libft/ft_isspace.c		\
				libft/ft_itoa.c			\
				libft/ft_lstadd_back.c	\
				libft/ft_lstadd_front.c	\
				libft/ft_lstclear.c		\
				libft/ft_lstdelone.c	\
				libft/ft_lstiter.c		\
				libft/ft_lstlast.c		\
				libft/ft_lstmap.c		\
				libft/ft_lstnew.c		\
				libft/ft_lstsize.c		\
				libft/ft_memchr.c		\
				libft/ft_memcmp.c		\
				libft/ft_memcpy.c		\
				libft/ft_memmove.c		\
				libft/ft_memset.c		\
				libft/ft_nbrlen.c		\
				libft/ft_putchar_fd.c	\
				libft/ft_putendl_fd.c	\
				libft/ft_putnbr_fd.c	\
				libft/ft_putstr_fd.c	\
				libft/ft_split.c		\
				libft/ft_strchr.c		\
				libft/ft_strcmp.c		\
				libft/ft_strcpy.c		\
				libft/ft_strdup.c		\
				libft/ft_striteri.c 	\
				libft/ft_strjoin.c 		\
				libft/ft_strlcat.c 		\
				libft/ft_strlcpy.c		\
				libft/ft_strlen.c		\
				libft/ft_strmapi.c		\
				libft/ft_strncmp.c		\
				libft/ft_strnstr.c		\
				libft/ft_strrchr.c		\
				libft/ft_strtrim.c		\
				libft/ft_substr.c		\
				libft/ft_tolower.c		\
				libft/ft_toupper.c		\
				ft_printf/ft_printf.c		\
				ft_printf/ft_print_char.c 	\
				ft_printf/ft_print_i_d.c 	\
				ft_printf/ft_print_x.c 		\
				ft_printf/ft_print_s.c 		\
				ft_printf/ft_print_p.c 		\
				ft_printf/ft_print_u.c	 	\
				ft_printf/ft_putnbr_base.c	\
				ft_printf/ft_fprintf.c		\
				ft_printf/ft_fprint_char.c 	\
				ft_printf/ft_fprint_num.c 	\
				ft_printf/ft_fprint_base.c 	\
				get_next_line/get_next_line.c		


LST_OBJS	:=	$(LST_SRCS:.c=.o)

LST_INCS	:=	libft.h get_next_line.h ft_printf.h
SRCS		:=	$(addprefix $(DIR_SRCS)/,$(LST_SRCS))
OBJS		:=	$(addprefix $(DIR_OBJS)/,$(LST_OBJS))
INCS		:=	$(addprefix $(DIR_INCS)/,$(LST_INCS))

ERASE		=	\033[A\033[2K\r
BLUE		=	\033[34m
YELLOW		=	\033[33m
GREEN		=	\033[32m
END			=	\033[0m

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c $(INCS) Makefile
	mkdir -p $(DIR_OBJS)
	mkdir -p $(DIR_OBJS)/get_next_line
	mkdir -p $(DIR_OBJS)/ft_printf
	mkdir -p $(DIR_OBJS)/libft
	$(CC) $(FLAGS) -I $(DIR_INCS) -c $< -o $@
	printf "$(ERASE)$(BLUE) > Compilation :$(END) $< \n"

all:	$(NAME)

$(NAME):	$(OBJS)
	ar rcs $@ $^
	printf "$(ERASE)$(GREEN)$@ made\n$(END)"

clean:
	rm -rf $(DIR_OBJS)
	printf "$(YELLOW)$(DIR_OBJS) libft removed$(END)\n"

fclean:		clean
	rm -rdf	$(NAME)
	printf "$(YELLOW)$(NAME) removed$(END)\n"

re:			fclean all

.PHONY:	all clean fclean re
.SILENT:
