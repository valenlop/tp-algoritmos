
// TDA: Laburar con imagenes ((Incompleto))

#ifndef IMAGEN_H
#define IMAGEN_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "color.h"


typedef struct imagen imagen_t;


void imagen_destruir(imagen_t *i);

imagen_t *imagen_crear(size_t ancho, size_t alto);

imagen_t *imagen_clonar(const imagen_t *i);

void imagen_escribir_ppm(const imagen_t *i);

imagen_t *imagen_leer_ppm();


// Esta parte es matriz.h, falta adaptar estas funciones si es necesario

// Tengo imagen_iniciar_color que hace lo mismo

void inicializar_matriz(size_t filas, size_t columnas, char matriz[filas][columnas]);

// Tengo imagen_clonar que hace lo mismo

void copiar_matriz(size_t filas, size_t columnas, char destino[filas][columnas], char origen[filas][columnas]);

// Tengo imagen_escribir_ppm que me hace los ppm con colores (mejor creo)

void imprimir_ppm(size_t filas, size_t columnas, char matriz[filas][columnas]);

// Tengo imagen_pegar_no_negros que es mas especifica, como esta seria general considero reutilizarla si veo que me puede servir

bool pegar_submatriz(size_t mfs, size_t mcs, char matriz[mfs][mcs], size_t sfs, size_t scs, char submatriz[sfs][scs], size_t fi, size_t ci);

// Esta no se para que me puede servir

int sumar_entorno(size_t filas, size_t columnas, char matriz[filas][columnas], size_t fc, size_t cc);

// Esta seguro no me sirve (tiene reglas muy especificas)

void evolucionar_celulas(size_t filas, size_t columnas, char final[filas][columnas], char inicial[filas][columnas]);

// Final matriz.h

// Posibles primitivas a agregar

/*falta:  - imagen_setear_pixel: Permite establecer el color de un pixel
            - imagen_obtener_pixel: Permite saber el color de un pixel
            - imagen_pegar_no_negros: Permite pegar pixeles diferentes al negro
            - imagen_rotar: Permite rotar una imagen 90° en sentido antihorario
            - imagen_iniciar_color: Permite iniciar una imagen con un color determinado
*/

bool imagen_setear_pixel(imagen_t *i, size_t fila, size_t columna, color_t color);

color_t imagen_obtener_pixel(imagen_t *i, size_t fila, size_t columna);

bool imagen_pegar_no_negros(imagen_t *destino, imagen_t *origen, size_t sf, size_t sc); 

imagen_t *imagen_rotar(imagen_t *i); // Tener en cuenta que crea una imagen nueva rotada

imagen_t *imagen_iniciar_color(size_t ancho, size_t alto, color_t color); // Tendriamos que discutir si va en este TDA o en otro



#endif