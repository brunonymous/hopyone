CC = gcc
BIN = hopyone
SRC  = main.c general.c hopy.c ini.c collisions.c scores.c font.c burst.c manage.c rope.c sound.c unzip_data.c unzip.c counter_shareware.c script_page.c
OBJ  = main.o general.o hopy.o ini.o collisions.o scores.o font.o burst.o manage.o rope.o sound.o unzip_data.o unzip.o counter_shareware.o script_page.o

ERROR_CFLAGS = -Wall -W -pedantic -std=c99
DEBUG_CFLAGS = -g3 -ggdb3 
SDL_CFLAGS = $$(sdl-config --cflags)
CFLAGS = $(ERROR_CFLAGS) $(DEBUG_CFLAGS) $(SDL_CFLAGS)
SDL_LDFLAGS = $$(sdl-config --libs)
#LDFLAGS = $(ERROR_CFLAGS) $(DEBUG_CFLAGS) $(SDL_LDFLAGS) -lSDL_ttf
LDFLAGS = $(DEBUG_CFLAGS) $(SDL_LDFLAGS) -lSDL_ttf -lSDL_mixer -lSDL_image

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(LDFLAGS) -o $(BIN) $(OBJ)

clean:
	rm -f *.o *~ core $(BIN) $(NET_BIN)
