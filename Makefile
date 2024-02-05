# Makefile for minishell
# Auother: Bobbie Syvasalmi
# Version 1.0
# Date: 01/28/2024

NAME = minishell
# HEADER = minishell.h

LIB = libft/libft.a
LIBDIR = ./libft
# READLINE_LIB = ~/.brew/opt/readline/lib
# READLINE_HEADER = ~/.brew/opt/readline/include
READLINE_LIB = /opt/homebrew/opt/readline/lib 
READLINE_HEADER = /opt/homebrew/opt/readline/include
SRC = 	minishell.c \
		minishell_prompt.c \
		minishell_history.c \
		minishell_utils.c \
		minishell_builtins.c \
		minishell_builtins_exit.c \
		pipex.c \
		pipex_execute.c \
		pipex_redirect.c \
		pipex_redirect_utils.c \
		pipex_interpret.c \
		pipex_env_variables.c \
		pipex_path.c \
		pipex_builtins.c \
		pipex_utils.c \
		pipex_command.c \
		pipex_errors.c 

OBJ = $(SRC:%.c=%.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror
DEBUG_FLAGS = -g -fsanitize=address,undefined,integer

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -I. -L$(LIBDIR) -lft -L $(READLINE_LIB) -I $(READLINE_HEADER) -lreadline -o $(NAME)

.phony: all
all: $(NAME)

# bonus: $(BONUS)

# $(BONUS): $(OBJ_BONUS)
# 	$(CC) $(CFLAGS) $(OBJ_BONUS) -L$(LIBDIR) -lft -o $(NAME)
# 	@touch .bonus

%.o: %.c $(LIB)
	$(CC) $(CFLAGS) -I. -I $(READLINE_HEADER) $< -c
	

$(LIB):
	make -C $(LIBDIR)

clean:
	rm -f .bonus
	rm -rf *.o
	make -C $(LIBDIR) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(LIB)
	
re: fclean all

.phony: all clean fclean re