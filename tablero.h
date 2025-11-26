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


// (1) tablero_crear: Crea un tablero nuevo con todos sus pixeles en negro

tablero_t *tablero_crear(size_t columnas, size_t filas);


// (2) tablero_destruir: Esta no se que tan indispensable sera

void tablero_destruir(tablero_t *t);


// (3) tablero_perdio: Deberia poder chequear si la pieza actual choco en la parte superior del tablero

bool tablero_perdio(tablero_t *t, pieza_t *p);


// (4) tablero_colision: Deberia poder chequear que una pieza choco con la arena o con el piso

bool tablero_colision(tablero_t *t, pieza_t *p); // Del TDA pieza_h puedo saber la posicion de p


// (5) tablero_pegar_pieza: usar "imagen_pegar_no_negros": Deberia recibir una pieza y pegar la imagen de la pieza en el tablero

bool tablero_pegar_pieza(tablero_t *t, pieza_t *p); // // Del TDA pieza_h puedo saber la posicion de p


// (6) tablero_simular_arena: Solo modifica los pixeles de un tablero

void tablero_simular_arena(tablero_t *t);


// (7) tablero_linea_formada: Recursiva, revisar que si se forma una linea hay que eliminar la linea

size_t tablero_linea_formada(tablero_t *t);

#endif