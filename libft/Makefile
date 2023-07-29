NAME = libft.a
HEAD = libft.h
CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
SRC = ft_atoi\
ft_bzero\
ft_calloc\
ft_isalnum\
ft_isalpha\
ft_isprint\
ft_isdigit\
ft_isascii\
ft_itoa\
ft_memchr\
ft_memcpy\
ft_memset\
ft_memcmp\
ft_memmove\
ft_split\
ft_strlcat\
ft_strlen\
ft_strnstr\
ft_strcmp\
ft_strrchr\
ft_strchr\
ft_strlcpy\
ft_strlcat\
ft_strdup\
ft_strncmp\
ft_strlcat\
ft_strtrim\
ft_substr\
ft_tolower\
ft_toupper\
ft_strmapi\
ft_striteri\
ft_strjoin\
ft_putchar_fd\
ft_putstr_fd\
ft_putendl_fd\
ft_putnbr_fd\

OBJ = $(SRC:=.o)

BNSSRC = ft_lstnew\
ft_lstadd_front\
ft_lstsize\
ft_lstlast\
ft_lstadd_back\
ft_lstdelone\
ft_lstclear\
ft_lstiter\
ft_lstmap
BNSOBJ = $(BNSSRC:.c=.o)
all:  $(NAME)

$(NAME): $(OBJ)
		ar rc $(NAME) $(OBJ)

%.o: %.c $(HEAD)
		$(CC) $(CFLAGS) -o $@ -c $<
 
bonus: all $(BNSOBJ)
		ar rc $(NAME) $(BNSOBJ)

clean:
		$(RM) $(OBJ) $(BNSOBJ)

fclean: clean
		$(RM) $(NAME)

re: fclean bonus

.PHONY: clean bonus fclean re