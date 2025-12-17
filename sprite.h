
// TDA: Laburar un sprite ((Terminado))

#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct sprite sprite_t;

// Contructor

sprite_t *sprite_crear(FILE *fi);

// Destructor

void sprite_destruir(sprite_t *s);

// Getters

size_t sprite_ancho(sprite_t *s);
size_t sprite_alto(sprite_t *s);

// Obtener etiqueta del sprite

char *sprite_etiqueta(const sprite_t *s);

// Obtener el valor de un pixel 

bool sprite_obtener(const sprite_t *s, size_t fila, size_t col);

// Setters

bool sprite_establecer(sprite_t *s, size_t fila, size_t col, bool valor);

// Primitiva

bool sprite_a_pbm(const sprite_t *s, FILE *fo);


#endif