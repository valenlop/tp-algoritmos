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


// (1) 

tablero_t *tablero_crear(size_t columnas, size_t filas);


// (2) 

void tablero_destruir(tablero_t *t);


// (3) 

bool tablero_perdio(tablero_t *t, pieza_t *p);


// (4) 

bool tablero_colision(tablero_t *t, pieza_t *p); 


// (5) 

bool tablero_pegar_pieza(tablero_t *t, pieza_t *p);


// (6) 

void tablero_simular_arena(tablero_t *t);

// (7) 

bool tablero_linea_formada(tablero_t *t, size_t *puntaje);

// (8) Getter

size_t tablero_alto(tablero_t *t);

// (9) Getter

size_t tablero_ancho(tablero_t *t);

// (10) Getter

imagen_t *tablero_imagen(tablero_t *t);



#endif