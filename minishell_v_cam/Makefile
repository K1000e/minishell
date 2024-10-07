
# Nom de l'exécutable
NAME = minihell

# Fichiers sources
SRCS = main.c parsing.c 

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Dossier où sont situées les bibliothèques (comme libft)
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

# Options de compilation
CC = gcc
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline


# Règle par défaut
all: $(NAME)

# Règle de compilation de l'exécutable
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)  $(LDFLAGS)

# Règle pour compiler les fichiers .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

# Règle pour construire libft si nécessaire
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Règle pour nettoyer les fichiers objets
clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

# Règle pour nettoyer les fichiers objets et l'exécutable
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

# Règle pour recompiler tout
re: fclean all

# Règles auxiliaires
.PHONY: all clean fclean re
