
// TDA: Laburar con imagenes ((Incompleto))

#ifndef IMAGEN_H
#define IMAGEN_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "color.h"


typedef struct imagen imagen_t;


void imagen_destruir(imagen_t *i); // (1)

imagen_t *imagen_crear(size_t ancho, size_t alto); // (2)

imagen_t *imagen_clonar(const imagen_t *i); // (3)

void imagen_escribir_ppm(const imagen_t *i); // (4)

// Lee de f una imagen en formato PPM RGB de 24 bits y la carga en memoria

imagen_t *imagen_leer_ppm(FILE *f); // (5) Creo que la acomode para leer de un archivo

bool imagen_setear_pixel(imagen_t *i, size_t fila, size_t columna, color_t color); // (6)

color_t imagen_obtener_pixel(imagen_t *i, size_t fila, size_t columna); // (7)

bool imagen_pegar_no_negros(imagen_t *destino, imagen_t *origen, size_t sf, size_t sc); // (8)

imagen_t *imagen_rotar(imagen_t *i); // (9)

void imagen_iniciar_color(imagen_t *i, color_t color); // (10)

size_t imagen_ancho(imagen_t *i); // (11)

size_t imagen_alto(imagen_t *i); // (12)



#endif