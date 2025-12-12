// TDA: Une todos los TDAs para simular el juego

#ifndef JUEGO_H
#define JUEGO_H

#include <stdio.h>

#include "imagen.h"
#include "sprites.h"

typedef struct juego juego_t;

//Constructor

juego_t *juego_crear(imagen_t *fondo, sprites_t *figuras);

void juego_destruir(juego_t *j);

size_t juego_get_puntaje(const juego_t *j);

bool juego_terminado(const juego_t *j);

#endif