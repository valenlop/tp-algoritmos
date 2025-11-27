//TDA: Trabajo con la pieza

#ifndef PIEZA_H
#define PIEZA_H

#include <stdint.h>
#include <stdbool.h>

#include "imagen.h"

typedef struct pieza pieza_t;

// Constructor

pieza_t *pieza_crear(imagen_t *img, size_t x, size_t y);

// Destructor

void pieza_destruir(pieza_t *p);

// Clon y copia

pieza_t *pieza_clonar(const pieza_t *p); // Quizas sirva

pieza_t *pieza_copiar(pieza_t *p_origen, const pieza_t *p_destino); // Devuelve p_destino

// Getters

const imagen_t *pieza_get_imagen(const pieza_t *p);

size_t pieza_get_x(const pieza_t *p);

size_t pieza_get_y(const pieza_t *p);

color_t pieza_get_color(const pieza_t *p);

color_t pieza_color_pixel(pieza_t *p, size_t x, size_t y);

size_t pieza_ancho(pieza_t *p);

size_t pieza_alto(pieza_t *p);

// Setters

void pieza_mover_x(pieza_t *p, size_t x);

void pieza_mover_y(pieza_t *p, size_t y);

void pieza_set_posicion(pieza_t *p, size_t x, size_t y); // Quizas sirva

bool pieza_rotar(pieza_t *p);

#endif