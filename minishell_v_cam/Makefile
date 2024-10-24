NAME		:=	minihell

CC			:=	cc
CFLAGS		:= -Wall -Wextra -Werror -g #-fsanitize=address 
LDFLAGS		:= -lreadline

#CFLAGS = -Wall -Wextra -Werror -I/opt/homebrew/opt/readline/include #-g -fsanitize=address 
#LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline

DIR_SRCS	:=	srcs
DIR_OBJS	:=	.objs
DIR_INCS	:=	include
DIR_LIBFT	:=	libft
DIR_PIPEX	:=	pipex
LIBFT_LIB	:=	$(DIR_LIBFT)/libft.a

LST_SRCS	:=	main.c			\
				bultins_env.c 	\
				bultins.c 		\
				environment.c	\
				check_errors.c 	\
				parsing.c 		\
				prompt.c 		\
				pipex_execute.c \
				pipex.c \
				pipex_open_free.c 

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
RED 		=	\033[31m


$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c $(INCS)
	@mkdir -p $(DIR_OBJS)
	$(CC) $(CFLAGS) -I $(DIR_INCS) -c $< -o $@
	printf "$(ERASE)$(BLUE) > Compilation :$(END) $< \n"


all: $(LIBFT_LIB) $(NAME)

$(LIBFT_LIB):
	@$(MAKE) -C $(DIR_LIBFT) --no-print-directory
	@echo "Finished compiling libft."

$(NAME):	$(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBFT_LIB) $(LDFLAGS)
	@$(MAKE) hell --no-print-directory
	
clean:
	@rm -rf $(DIR_OBJS)
	$(MAKE) -C $(DIR_LIBFT) clean --no-print-directory
	printf "$(YELLOW)$(DIR_OBJS) removed$(END)\n"

fclean:		clean
	rm -rdf	$(NAME)
	rm -rdf	libft/libft.a
	printf "$(YELLOW)$(NAME) & libft removed$(END)\n"

re:			fclean all

hell: 
	@echo "$(RED)$(ERASE)                            (                                   "
	@echo "                .            )        )                         "
	@echo "                         (  (|              .                   "
	@echo "                     )   )\/ ( ( (                              "
	@echo "             *  (   ((  /     ))\))  (  )    )                  "
	@echo "           (     \   )\(          |  ))( )  (|                  "
	@echo "           >)     ))/   |          )/  \((  ) \                 "
	@echo "           (     (      .        -.     V )/   )(    (          "
	@echo "            \   /     .   \            .       \))   ))         "
	@echo "              )(      (  | |   )            .    (  /           "
	@echo "             )(    , ))     \ /          \(  .    )             "
	@echo "             (\>  , /__      ))            __ .  /              "
	@echo "            ( \   | /  ___   ( \/     ___   \ | ( (             "
	@echo "             \.)  |/  /   \__      __/   \   \|  ))             "
	@echo "            .  \. |>  \      | __ |      /   <|  /              "
	@echo "                 )/    \____/ :..: \____/     \ <               "
	@echo "          )   \ (|__  .      / ;: \          __| )  (           "
	@echo "         ((    )\)  ~--_     --  --      _--~    /  ))          "
	@echo "          \    (    |  ||               ||  |   (  /            "
	@echo "                \.  |  ||_             _||  |  /                "
	@echo "                  > :  |  ~V+-I_I_I-+V~  |  : (.                "
	@echo "                 (  \:  T\   _     _   /T  : ./                 "
	@echo "                  \  :    T^T T-+-T T^T    ;<                   "
	@echo "                   \.. _       -+-       _   )                  "
	@echo "         )            .  --=.._____..=-- . ./         (         "
	@echo "        ((     ) (          )             (     ) (   )>        "
	@echo "         > \/^/) )) (   ( /(.      ))     ))._/(__))./ (_.      "
	@echo "        (  _../ ( \))    )   \ (  / \.  ./ ||  ..__:|  _. \     "
	@echo "        |  \__.  ) |   (/  /: :)) |   \/   |(  <.._  )|  ) )    "
	@echo "       ))  _./   |  )  ))  __  <  | :(     :))   .//( :  : |    "
	@echo "       (: <     ):  --:   ^  \  )(   )\/:   /   /_/ ) :._) :    "
	@echo "        \..)   (_..  ..  :    :  : .(   \..:..    ./__.  ./     "
	@echo "	 ( /(       )\ )  )\ )  )\ )  ( /(       )\ )  )\ )  "
	@echo "	 )\()) (   (()/( (()/( (()/(  )\()) (   (()/( (()/(  "
	@echo "	((_)\  )\   /(_)) /(_)) /(_))((_)\  )\   /(_)) /(_)) "
	@echo "	 _((_)((_) (_))  (_))  (_))   _((_)((_) (_))  (_))   "
	@echo "	| || || __|| |   | |   / __| | || || __|| |   | |    "
	@echo "	| __ || _| | |__ | |__ \__ \ | __ || _| | |__ | |__  "
	@echo "	|_||_||___||____||____||___/ |_||_||___||____||____| "
	@echo "	                                                     "


.PHONY:	all clean fclean re
.SILENT: 
