NAME = example
FILES = prompt_line
SRCS = $(patsubst %, sources/%.c, $(FILES))
BINS = $(patsubst %, %.o, $(FILES))
FLAGS = -g -Wall -Wextra -Werror

.PHONY: all clean fclean

%.o : sources/%.c
	gcc -Iincludes $(FLAGS) -c $<

all: $(NAME)

$(NAME): $(BINS)
	gcc -Iincludes $(FLAGS) $(LIBFT) $(BINS) main.c -o $(NAME)

clean:
	/bin/rm -f $(BINS)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
