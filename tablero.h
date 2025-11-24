//

#ifndef TABLERO_H
#define TABLERO_H

#include <stddef.h>

#include "color.h"
#include "imagen.h"

typedef struct tablero tablero_t;

// Primitivas

// tablero_crear: Crea un tablero nuevo con todos sus pixeles en negro

tablero_t *tablero_crear(size_t columnas, size_t filas);

// tablero_destruir: Esta no se que tan indispensable sera

// tablero_pegar_pieza: usar "imagen_pegar_no_negros"

// tablero_simular_arena

// tablero_linea_formada

#endif