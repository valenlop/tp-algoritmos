
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

size_t sprite_ancho(const sprite_t *s);
size_t sprite_alto(const sprite_t *s);
char *sprite_etiqueta(const sprite_t *s);
bool sprite_obtener(const sprite_t *s, size_t fila, size_t col);

// Setters

bool sprite_establecer(sprite_t *s, size_t fila, size_t col, bool valor);

// Primitiva

bool sprite_a_pbm(const sprite_t *s, FILE *fo);


#endif