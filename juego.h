// TDA: Une todos los TDAs para simular el juego

#ifndef JUEGO_H
#define JUEGO_H

#include <stdio.h>

#include "imagen.h"
#include "sprites.h"
#include "tablero.h"

typedef struct juego juego_t;

//Constructor

juego_t *juego_crear(imagen_t *fondo, sprites_t *figuras);

// Destructor

void juego_destruir(juego_t *j);

// Getters

imagen_t *juego_obtener_frame(juego_t *j);

tablero_t *juego_obtener_tablero(juego_t *j);

pieza_t *juego_obtener_pieza_actual(juego_t *j);

pieza_t *juego_obtener_pieza_siguiente(juego_t *j);

size_t juego_obtener_puntaje(juego_t *j);

size_t juego_obtener_clears(juego_t *j);

double juego_obtener_velocidad_actual(juego_t *j);

uint32_t juego_obtener_minutos(juego_t *j);

uint32_t juego_obtener_segundos(juego_t *j);

uint32_t juego_obtener_milisegundos(juego_t *j);

bool juego_terminado(juego_t *j);

// Setters


// La pieza siguiente que le paso tiene que estar bien dimensionada
void juego_setear_pieza_actual(juego_t *j, pieza_t *pieza_siguiente_real);

void juego_setar_pieza_siguiente(juego_t *j, pieza_t *pieza_creada);

void juego_aumentar_nro_clears(juego_t *j);

void juego_aumentar_puntaje(juego_t *j, size_t cantidad);

void juego_aumentar_velocidad_actual(juego_t *j);

void juego_setear_minutos(juego_t *j, uint32_t minutos);

void juego_setear_segundos(juego_t *j, uint32_t segundos);

void juego_setear_milisegundos(juego_t *j, uint32_t milisegundos);

// Primitivas

imagen_t *juego_generar_frame(juego_t *j, sprites_t *ss);

#endif