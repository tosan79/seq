SRC = avll_i.cpp
OBJ = $(SRC:.cpp=.o)
PRG = avll
CC = /opt/homebrew/bin/x86_64-w64-mingw32-g++
FLAGS = -std=c++17 -Wall -g

all: $(PRG)

$(PRG): $(OBJ)
	$(CC) $(OBJ) -o $(PRG) $(FLAGS)

$(OBJ): $(SRC)
	$(CC) -c $(SRC) -o $(OBJ) $(FLAGS)

clean:
	rm $(OBJ) $(PRG)
