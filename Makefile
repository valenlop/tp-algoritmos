CFLAGS = -Wall -Werror -std=c99 -pedantic -g

SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs)

sandtrix: main.o juego.o tablero.o sprite.o sprites.o color.o pieza.o imagen.o
	gcc $(CFLAGS) main.o juego.o tablero.o sprite.o sprites.o color.o pieza.o imagen.o -o sandtrix $(SDL_LIBS) -lm

main.o: main.c juego.h config.h
	gcc -c main.c $(CFLAGS) $(SDL_CFLAGS)

juego.o: juego.c juego.h imagen.h sprites.h tablero.h
	gcc -c juego.c $(CFLAGS)

tablero.o: tablero.c tablero.h color.h imagen.h pieza.h
	gcc -c tablero.c $(CFLAGS)

sprite.o: sprite.c sprite.h 
	gcc -c sprite.c $(CFLAGS)

sprites.o: sprites.c sprites.h
	gcc -c sprites.c $(CFLAGS)

color.o: color.c color.h
	gcc -c color.c $(CFLAGS)

pieza.o: pieza.c pieza.h imagen.h sprite.h sprites.h
	gcc -c pieza.c $(CFLAGS)

imagen.o: imagen.c imagen.h color.h
	gcc -c imagen.c $(CFLAGS)

clean: 
	rm *.o sandtrix