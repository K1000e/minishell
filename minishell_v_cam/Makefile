NAME		:=	minihell

CC			:=	cc
CFLAGS		:= -Wall -Wextra -Werror -g #-fsanitize=address 
LDFLAGS		:= -lreadline

DIR_SRCS	:=	srcs
DIR_OBJS	:=	.objs
DIR_INCS	:=	include
DIR_LIBFT	:=	libft
LIBFT_LIB	:=	$(DIR_LIBFT)/libft.a

LST_SRCS	:=	main.c		\
				parsing.c 	\
				bultins.c

LST_OBJS	:=	$(LST_SRCS:.c=.o)
LST_OBJS_B	:=	$(LST_SRCS_B:.c=.o)

LST_INCS	:=	minihell.h

SRCS		:=	$(addprefix $(DIR_SRCS)/,$(LST_SRCS)) \
				pipex_execute.c \
				pipex_main.c \
				pipex_open_free.c
OBJS		:=	$(addprefix $(DIR_OBJS)/,$(LST_OBJS))
INCS		:=	$(addprefix $(DIR_INCS)/,$(LST_INCS))

ERASE		=	\033[A\033[2K\r
BLUE		=	\033[34m
YELLOW		=	\033[33m
GREEN		=	\033[32m
END			=	\033[0m

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c $(INCS)
	@mkdir -p $(DIR_OBJS)
	$(CC) $(CFLAGS) -I $(DIR_INCS) -c $< -o $@
	printf "$(ERASE)$(BLUE) > Compilation :$(END) $< \n"


all: $(LIBFT_LIB) $(NAME)

$(LIBFT_LIB):
	@$(MAKE) -C $(DIR_LIBFT) --no-print-director
	@echo "Finished compiling libft."

$(NAME):	$(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ libft/libft.a $(LDFLAGS)
	printf "$(ERASE)$(GREEN)$@ made\n$(END)"

clean:
	@rm -rf $(DIR_OBJS)
	$(MAKE) -C $(DIR_LIBFT) clean --no-print-director
	printf "$(YELLOW)$(DIR_OBJS) removed$(END)\n"

fclean:		clean
	rm -rdf	$(NAME)
	rm -rdf	libft/libft.a
	printf "$(YELLOW)$(NAME) & checker & libft removed$(END)\n"

re:			fclean all

.PHONY:	all clean fclean re
.SILENT: 
