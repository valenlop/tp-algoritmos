//TDA: Trabajo con la pieza

#ifndef PIEZA_H
#define PIEZA_H

#include <stdint.h>
#include <stdbool.h>

#include "imagen.h"

typedef struct pieza pieza_t;

// Constructor

pieza_t *pieza_crear(imagen_t *img, int x, int y, char tipo, uint8_t e); // Esta firma para mi esta bien, actualizar en la struct lo de x e y

// Destructor

void pieza_destruir(pieza_t *p);

// Clon

pieza_t *pieza_clonar(const pieza_t *p);

// Getters

const imagen_t *pieza_get_imagen(const pieza_t *p);

size_t pieza_get_x(const pieza_t *p);

size_t pieza_get_y(const pieza_t *p);

char pieza_get_tipo(const pieza_t *p);

uint8_t pieza_get_e(const pieza_t *p);

color_t pieza_color_pixel(pieza_t *p, size_t x, size_t y); // Falta implementar esto (lo necesito para tablero.h)

size_t pieza_ancho(pieza_t *p); // Falta implementar esto (lo necesito para tablero.h)

size_t pieza_alto(pieza_t *p); // Falta implementar esto (lo necesito para tablero.h)

// Setters

void pieza_mover(pieza_t *p, size_t x, size_t y);

void pieza_set_posicion(pieza_t *p, size_t x, size_t y);

bool pieza_rotar(pieza_t *p);

#endif