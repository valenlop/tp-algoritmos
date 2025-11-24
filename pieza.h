//TDA: Trabajo con la pieza

#ifndef PIEZA_H
#define PIEZA_H

#include <stdint.h>
#include <stdbool.h>

#include "imagen.h"

typedef struct pieza pieza_t;

// Constructor

pieza_t *pieza_crear(imagen_t *img, int x, int y, char tipo, uint8_t e);

// Destructor

void pieza_destruir(pieza_t *p);

// Clon

void pieza_clonar(const pieza_t *p, pieza_t *p_clon);

// Getters

const imagen_t *pieza_get_imagen(const pieza_t *p);

int pieza_get_x(const pieza_t *p);

int pieza_get_y(const pieza_t *p);

char pieza_get_caracter(const pieza_t *p);

uint8_t pieza_get_e(const pieza_t *p);

// Setters

void pieza_set_posicion(pieza_t *p, int x, int y);

void pieza_mover(pieza_t *p, int x, int y);

bool pieza_rotar(pieza_t *p);

#endif