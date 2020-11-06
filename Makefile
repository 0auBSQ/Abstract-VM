NAME = abstractvm
SRC = Exceptions.cpp main.cpp OperandsFactory.cpp
INC = main.hpp Exceptions.hpp Operands.hpp IOperand.hpp
OBJ = $(addsuffix .o,$(basename $(SRC)))
CC = g++
CFLAGS = -Wall -Wextra -Werror -I. -g3
LIBS =

all: $(NAME)

$(NAME): $(OBJ) $(INC)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS)

%.o : %.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
