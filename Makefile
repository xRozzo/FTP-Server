##
## EPITECH PROJECT, 2022
## Makefile
## File description:
## Makefile
##

SRC 	=  $(wildcard *.c)

NAME	=	./myftp

OBJ	=	 $(SRC:.c=.o)

CFLAGS = -g3 -std=gnu17 -Wall -Wextra

all: $(NAME)

$(NAME): $(OBJ)
	gcc -g3 -o $(NAME) $(CGLAGS) $(OBJ)


clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
re: fclean all
