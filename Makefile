NAME = minishell
CC = cc 
FLAGS = -Wall -Werror -Wextra -lreadline 
AR = ar -r
RM = rm -f
HEAD = minishell.h
MK = make
LIBFT = ./libft/libft.a
HEADLIBFT = ./libft/libft.h
PRINTF = ./ft_printf/ft_printf.a
HEADPRINTF = ./ft_printf/ft_printf.h

LIBFTSRC = ./libft/ft_atoi.c\
		./libft/ft_bzero.c\
		./libft/ft_calloc.c\
		./libft/ft_isalnum.c\
		./libft/ft_isalpha.c\
		./libft/ft_isprint.c\
		./libft/ft_isdigit.c\
		./libft/ft_isascii.c\
		./libft/ft_itoa.c\
		./libft/ft_memchr.c\
		./libft/ft_memcpy.c\
		./libft/ft_memset.c\
		./libft/ft_memcmp.c\
		./libft/ft_memmove.c\
		./libft/ft_split.c\
		./libft/ft_strlcat.c\
		./libft/ft_strlen.c\
		./libft/ft_strnstr.c\
		./libft/ft_strrchr.c\
		./libft/ft_strchr.c\
		./libft/ft_strlcpy.c\
		./libft/ft_strlcat.c\
		./libft/ft_strdup.c\
		./libft/ft_strncmp.c\
		./libft/ft_strlcat.c\
		./libft/ft_strtrim.c\
		./libft/ft_substr.c\
		./libft/ft_tolower.c\
		./libft/ft_toupper.c\
		./libft/ft_strmapi.c\
		./libft/ft_striteri.c\
		./libft/ft_strjoin.c\
		./libft/ft_putchar_fd.c\
		./libft/ft_putstr_fd.c\
		./libft/ft_putendl_fd.c\
		./libft/ft_putnbr_fd.c\
		./libft/ft_strcmp.c\
		./libft/libft.h\

PRINTFSRC = ./ft_printf/ft_printf.c\
		./ft_printf/ft_putchar.c\
		./ft_printf/ft_puthex.c\
		./ft_printf/ft_putnbr.c\
		./ft_printf/ft_putstr.c\
		./ft_printf/ft_putunsignednbr.c\
		./ft_printf/ft_printf.h

SRC = parcing/handle_input.c\
		parcing/handle_operators.c\
		parcing/redirect.c\
		parcing/fill_cmd_list.c\
		parcing/split_input.c\
		execution/execute.c\
		execution/built_in/echo.c\
		execution/built_in/cd.c\
		execution/built_in/pwd.c\
		execution/built_in/env.c\
		execution/built_in/export.c\
		execution/built_in/exit.c\
		execution/built_in/unset.c\
		utils/args_double_pointer.c\
		utils/double_pointer_args.c\
		utils/params_utils.c\
		utils/garbage.c\
		main.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(LIBFT): $(LIBFTSRC)
	$(MK) -C ./libft

$(PRINTF): $(PRINTFSRC)
	$(MK) -C ./ft_printf

$(NAME): $(LIBFT) $(PRINTF) $(OBJ)
		cc  $(OBJ) -o $(NAME)  -L./libft -lft -L./ft_printf ./ft_printf/ft_printf.a $(FLAGS)
%.o : %.c $(HEAD)
	$(CC) -Wall -Wextra -Werror -g -c $< -o $@
clean:
		$(RM) $(OBJ)
		$(RM) main.o
		$(MK) clean -C ./libft
		$(MK) clean -C ./ft_printf

fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIBFT)
	$(RM) $(PRINTF)
	

re: fclean all

.PHONY: all bonus clean fclean re