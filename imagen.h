
// TDA: Crear imagenes y escribir ppm. Tal vez deba agregar y adaptar las funciones de matriz.h

// Deberia agregar: establecer u obtener el color de un pixel, rotar, pegar pixeles no negros, inicializar con un color determinado

#ifndef IMAGEN_H
#define IMAGEN_H

#include <stdint.h>
#include <stddef.h>

#include "color.h"


typedef struct imagen imagen_t;


void imagen_destruir(imagen_t *i);

imagen_t *imagen_crear(size_t ancho, size_t alto);

imagen_t *imagen_clonar(const imagen_t *i);

void imagen_escribir_ppm(const imagen_t *i);

imagen_t *imagen_leer_ppm();

#endif