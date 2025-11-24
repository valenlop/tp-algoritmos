
// TDA: Laburar con sprites ((Terminado))

#ifndef SPRITES_H
#define SPRITES_H

#include <stddef.h>

#include "sprite.h"

typedef struct sprites sprites_t;

// Constructor

sprites_t *sprites_crear(const char *ruta);

// Destructor

void sprites_destruir(sprites_t *ss);

// Getter

size_t sprites_cantidad(const sprites_t *ss);

// Primitiva

sprite_t *sprites_obtener(const sprites_t *ss, const char *etiqueta);

#endif