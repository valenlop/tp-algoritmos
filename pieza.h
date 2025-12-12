// Revisar (( No se si esta terminado))

//TDA: Trabajo con la pieza

#ifndef PIEZA_H
#define PIEZA_H

#include <stdint.h>
#include <stdbool.h>

#include "imagen.h"
#include "sprite.h"

typedef struct pieza pieza_t;

// Constructor

pieza_t *pieza_crear(sprite_t *tetramino, size_t fila, size_t columna);

// Destructor

void pieza_destruir(pieza_t *p);

// Getters

const imagen_t *pieza_get_imagen(const pieza_t *p);

size_t pieza_get_fila(const pieza_t *p);

size_t pieza_get_columna(const pieza_t *p);

color_t pieza_color_pixel(pieza_t *p, size_t fila, size_t columna);

size_t pieza_ancho(pieza_t *p);

size_t pieza_alto(pieza_t *p);

// Setters

void pieza_mover_fila(pieza_t *p, size_t cantidad);

void pieza_mover_columna(pieza_t *p, size_t cantidad);

void pieza_set_posicion(pieza_t *p, size_t fila, size_t columna); // Quizas sirva

bool pieza_rotar(pieza_t *p);

#endif