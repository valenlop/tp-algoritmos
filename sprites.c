
#include "sprites.h"

#include <string.h>
#include <stdlib.h>

static char *strdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *copy = malloc(n);
    if (copy == NULL) return NULL;
    memcpy(copy, s, n);
    return copy;
}


struct sprites {

    // sprites es un arreglo de punteros a sprite_t, donde viven los sprites (etiqueta, ancho, alto y su imagen)
    // etiquetas tiene la etiqueta de cada sprite de sprites, la etiqueta i corresponde a la etiqueta del sprite i
    // etiquetas == NULL => sprites == NULL
    // cant_sprites tiene la cantidad de sprites que hay en el contenedor
    // cant_etiquetas tiene la cantidad de etiquetas que hay en el contenedor
    // normalmente cant_sprites = cant_etiquetas pero si falla algun pedido de memoria pueden ser diferentes
    sprite_t **sprites;
    char **etiquetas;
    size_t cant_sprites, cant_etiquetas;
};


// (1)

sprites_t *sprites_crear(const char *ruta){
    FILE *f = fopen(ruta, "rb");
    if(f == NULL) return NULL;

    sprites_t *sprites = malloc(sizeof(sprites_t));
    if(sprites == NULL) return NULL;

    sprite_t *sprite;
    sprites->cant_sprites = 0;
    sprites->cant_etiquetas = 0;

    if((sprite = sprite_crear(f)) != NULL){

        sprites->sprites = malloc(sizeof(sprite_t *));
        if(sprites->sprites == NULL){
            free(sprites);
            return NULL;
        }
        
        sprites->cant_sprites++;

        sprites->etiquetas = malloc(sizeof(char *));
        if(sprites->etiquetas == NULL){
            free(sprites->sprites);
            free(sprites);
            return NULL;
        }
        
        sprites->cant_etiquetas++;

        sprites->sprites[0] = sprite;
        sprites->etiquetas[0] = strdup(sprite_etiqueta(sprite));
        
        
        
    }
    else {
        free(sprites);
        return NULL;
    }

    
    while((sprite = sprite_crear(f)) != NULL){

        sprite_t **aux = realloc(sprites->sprites, sizeof(sprite_t *) * (sprites->cant_sprites + 1));
        if(aux == NULL){
            sprites_destruir(sprites);
            return NULL;
        }

        sprites->sprites = aux;

        sprites->sprites[sprites->cant_sprites] = sprite;

        sprites->cant_sprites++;

        char **aux2 = realloc(sprites->etiquetas, sizeof(char *) * (sprites->cant_etiquetas + 1));
        if(aux2 == NULL){
            sprites_destruir(sprites);
            return NULL;
        }

        sprites->etiquetas = aux2;

        sprites->etiquetas[sprites->cant_etiquetas] = strdup(sprite_etiqueta(sprite));

        sprites->cant_etiquetas++;
    }

    fclose(f);

    return sprites;

}

// (2)

void sprites_destruir(sprites_t *ss){
    for(size_t s = 0; s < ss->cant_sprites; s++){
        sprite_destruir(ss->sprites[s]);
    }

    free(ss->sprites);

    for(size_t e = 0; e < ss->cant_etiquetas; e++){
        free(ss->etiquetas[e]);
    }

    free(ss->etiquetas);

    free(ss);
}

// (3)

size_t sprites_cantidad(const sprites_t *ss){
    return ss->cant_sprites;
}

// (4)


sprite_t *sprites_obtener(const sprites_t *ss, const char *etiqueta){
    for(size_t e = 0; e < ss->cant_sprites; e++){
        if(strcmp(etiqueta, ss->etiquetas[e]) == 0){
            return ss->sprites[e];
        }
    }
    return NULL;
}