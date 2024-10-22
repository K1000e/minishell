NAME		:=	pipex

CFLAGS		:=	-Wall -Werror -Wextra #-ggdb3 -g -fsanitize=address

DIR_SRCS	:=	srcs
DIR_OBJS	:=	.objs
DIR_INCS	:=	includes
LIBFT_LIB	:=	../libft.a

LST_SRCS	:=	pipex_main.c 		\
				pipex_open_free.c	\
				pipex_execute.c

LST_OBJS	:=	$(LST_SRCS:.c=.o)
LST_OBJS_B  :=	$(LST_SRCS:.c=.o)

LST_INCS	:=	pipex.h
SRCS		:=	$(addprefix $(DIR_SRCS)/,$(LST_SRCS))
OBJS		:=	$(addprefix $(DIR_OBJS)/,$(LST_OBJS))
INCS		:=	$(addprefix $(DIR_INCS)/,$(LST_INCS))

ERASE		=	\033[A\033[2K\r
BLUE		=	\033[34m
YELLOW		=	\033[33m
GREEN		=	\033[32m
END			=	\033[0m

ifeq ($(BUILD), bonus)
    FLAGS := $(CFLAGS) -D BONUS=1
else
    FLAGS := $(CFLAGS)
endif

all: $(LIBFT_LIB)
	@if [ ! -f .build_type ] || [ "$$(cat .build_type)" != "normal" ]; then \
		echo "normal" > .build_type; \
		$(MAKE) BUILD=normal $(NAME); \
	else \
		$(MAKE) BUILD=normal check_and_build; \
	fi

bonus: $(LIBFT_LIB)
	@if [ ! -f .build_type ] || [ "$$(cat .build_type)" != "bonus" ]; then \
		echo "bonus" > .build_type; \
		$(MAKE) BUILD=bonus $(NAME); \
	else \
		$(MAKE) BUILD=bonus check_and_build; \
	fi

check_and_build: $(OBJS)
	@if [ "$$(find $(DIR_OBJS) -type f -name '*.o' -newer $(NAME))" != "" ]; then \
		$(MAKE) BUILD=$(BUILD) $(NAME); \
	else \
		echo "$(BLUE)$(NAME) is up to date."; \
	fi

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c $(INCS) Makefile .build_type
	mkdir -p $(DIR_OBJS)
	$(CC) $(FLAGS) -I $(DIR_INCS) -c $< -o $@
	printf "$(ERASE)$(BLUE) > Compilation :$(END) $< \n"

$(NAME):	$(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@ libft/libft.a
	printf "$(ERASE)$(GREEN)$@ made\n$(END)"

clean:
	rm -rdf $(DIR_OBJS) .build_type
	printf "$(YELLOW) objets and .build_typeremoved$(END)\n"

fclean:		clean
	$(RM)	$(NAME)
	printf "$(YELLOW)$(NAME) and libft.a removed$(END)\n"

re:			fclean all

.PHONY:	all clean fclean re
.SILENT:
