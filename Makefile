NAME = libpure_c.a

SRC = $(wildcard src/*c)
OBJ = $(SRC:.c=.o)


CFLAGS = -Iinclude 


$(NAME): $(OBJ)
	ar rsc $(NAME) $(OBJ)


clean:
	@rm -rfv $(OBJ)

fclean: clean
	@rm -rfv $(NAME)


re: fclean $(NAME)
	


.PHONY: clean re fclean
.SECONDARY: $(OBJ)
