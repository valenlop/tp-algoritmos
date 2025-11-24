//

#ifndef TABLERO_H
#define TABLERO_H

#include <stddef.h>
#include <stdbool.h>

#include "color.h"
#include "imagen.h"
#include "pieza.h"

typedef struct tablero tablero_t;

// Primitivas

// tablero_crear: Crea un tablero nuevo con todos sus pixeles en negro

tablero_t *tablero_crear(size_t columnas, size_t filas);

// tablero_destruir: Esta no se que tan indispensable sera

// tablero_perdio

bool tablero_colision(tablero_t *t, pieza_t *p, size_t x, size_t y); // x = columna e y = fila

// tablero_pegar_pieza: usar "imagen_pegar_no_negros"

// tablero_simular_arena

size_t tablero_linea_formada(tablero_t *t);

#endif