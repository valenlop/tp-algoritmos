// TDA: Une todos los TDAs para simular el juego

#ifndef JUEGO_H
#define JUEGO_H

#include <stdbool.h>
#include <stdint.h>

#include "pieza.h"
#include "tablero.h"
#include "imagen.h"
#include "color.h"

typedef struct juego juego_t;

//Constructor

juego_t *juego_crear(size_t filas, size_t columnas, double fps);

//Destructor

void juego_destruir(juego_t *j);

bool juego_avanzar(juego_t *j, uint32_t ticks_ms);

void juego_input_izquierda(juego_t *j);
void juego_input_derecha(juego_t *j);
void juego_input_rotar(juego_t *j);
void juego_input_bajar(juego_t *j);

//Getters

const tablero_t *juego_get_tablero(const juego_t *j);

const pieza_t *juego_get_pieza_actual(const juego_t *j);

const pieza_t *juego_get_pieza_siguiente(const juego_t *j);

uint32_t juego_get_puntaje(const juego_t *j);

uint32_t juego_get_lineas(const juego_t *j);

uint32_t juego_get_tiempo_ms(const juego_t *j);

double juego_get_velocidad_actual(const juego_t *j);

bool juego_finalizado(const juego_t *j);


//Funciones que pueden ser utiles

size_t juego_pieza_actual_x(const juego_t *j);
size_t juego_pieza_actual_y(const juego_t *j);

const imagen_t *juego_get_sprite_siguiente(const juego_t *j);
color_t juego_get_color_siguiente(const juego_t *j);

#endif