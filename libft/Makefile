# Makefile for libft
# Auother: Bobbie Syvasalmi
# Version 1.0
# Date: 31/10/2023

# Usage:
#	make basic	- build libft.a without bonus
#	make bonus	- adds the bonus to libft.a
#	make all	- basic + bonus
#	make clean	- deletes the objects and the objs folder
#	make fclean	- clean + deletes libft.a
#	make re		- fclean + all

# List of source files
HEADER = libft.h 
#		printf/ft_printf_bonus.h \
#		get_next_line/get_next_line.h

SRCS = 	ft_atoi.c \
		ft_atod.c \
		ft_bzero.c \
		ft_calloc.c \
		ft_isalnum.c \
		ft_isalpha.c \
		ft_isascii.c \
		ft_isdigit.c \
		ft_isprint.c \
		ft_isspace.c \
		ft_itoa.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_memset.c \
		ft_power.c \
		ft_putchar_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_putstr_fd.c \
		ft_split.c \
		ft_strchr.c \
		ft_strdup.c \
		ft_striteri.c \
		ft_strjoin.c \
		ft_strlcat.c \
		ft_strlcpy.c \
		ft_strlen.c \
		ft_strmapi.c \
		ft_strncmp.c \
		ft_strnstr.c \
		ft_strrchr.c \
		ft_strtrim.c \
		ft_substr.c \
		ft_tolower.c \
		ft_toupper.c

BONUS_SRCS = 	ft_lstadd_back_bonus.c \
				ft_lstadd_front_bonus.c \
				ft_lstclear_bonus.c \
				ft_lstdelone_bonus.c \
				ft_lstiter_bonus.c \
				ft_lstlast_bonus.c \
				ft_lstmap_bonus.c \
				ft_lstnew_bonus.c \
				ft_lstsize_bonus.c

GNL_SRCS = get_next_line/get_next_line.c 

PRINTF_SRCS =	printf/ft_printf_bonus.c \
				printf/ft_printf_cs_bonus.c \
				printf/ft_printf_dipux_bonus.c \
				printf/ft_itoa_type_bonus.c \
				printf/ft_printf_helpers_bonus.c

#The objects
OBJS = $(SRCS:%.c=%.o)
BONUS_OBJS = $(BONUS_SRCS:%.c=%.o)
GNL_OBJS = $(GNL_SRCS:%.c=%.o)
PRINTF_OBJS = $(PRINTF_SRCS:%.c=%.o)

# Libary name
NAME = libft.a 

# CC Flags
CFLAGS = -Wall -Wextra -Werror


# Default rule - All
.phony = all
all: basic bonus gnl printf

# 
$(NAME): all

# Only basic
.phony = basic
basic: $(OBJS)

# Libft bonus
.phony = bonus
bonus: $(BONUS_OBJS)

# Get Next Line
.phony = gnl
gnl: $(GNL_OBJS)

# Printf
.phony = printf
printf: $(PRINTF_OBJS)

# Check each _bonus.o and recreate if there is 
%.o: %.c
	@cc $(CFLAGS) -c $< -I $(HEADER) -o $@
	@ar rc $(NAME)$@
	@ranlib $(NAME)

# Clean up
.phony = clean
clean: 
	rm -rf $(OBJS) $(BONUS_OBJS) $(GNL_OBJS) $(PRINTF_OBJS)

# Full clean up
.phony = fclean
fclean: clean
	rm -rf $(NAME)

# Clean up and re-create
.phony = re
re: fclean all
