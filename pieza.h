// Revisar (( No se si esta terminado))

//TDA: Trabajo con la pieza

#ifndef PIEZA_H
#define PIEZA_H

#include <stdint.h>
#include <stdbool.h>

#include "imagen.h"
#include "sprite.h"
#include "sprites.h"

typedef struct pieza pieza_t;

// Constructor

pieza_t *pieza_crear(sprites_t *tetraminos);

// Destructor

void pieza_destruir(pieza_t *p);

// Getters

imagen_t *pieza_get_imagen(const pieza_t *p);

size_t pieza_get_fila(const pieza_t *p);

size_t pieza_get_columna(const pieza_t *p);

color_t pieza_color_pixel(pieza_t *p, size_t fila, size_t columna);

size_t pieza_ancho(pieza_t *p);

size_t pieza_alto(pieza_t *p);

// Setters

// Necesito una funcion para colorear las piezas, 

void pieza_colorear(pieza_t *p, color_t color, bool random);

void pieza_mover_fila(pieza_t *p, size_t cantidad);

void pieza_mover_columna(pieza_t *p, int cantidad);

void pieza_set_posicion(pieza_t *p, size_t fila, size_t columna); // Quizas sirva

bool pieza_rotar(pieza_t *p);

// Primitivas

pieza_t *pieza_a_version_chica(pieza_t *p, sprites_t *ss);

pieza_t *pieza_crear_tubo(pieza_t *p, sprites_t *ss);

pieza_t *pieza_crear_dos_puntos(sprites_t *ss);

#endif