CC 	= gcc
SRC = main.c 
DST = bin/game_of_life

SDL_FLAGS = -I include -L lib -lmingw32 -lSDL2main -lSDL2

compile:
	$(CC) $(SRC) -o $(DST) $(SDL_FLAGS)

run:
	./$(DST)

clean:
	rm -rf bin/*.exe

all: compile run
