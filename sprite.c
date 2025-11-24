
#include "sprite.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


struct sprite {

    // etiqueta es la etiqueta del sprite
    // ancho y alto son parametros del sprite
    // pixeles es el lugar donde viven los bits representativos del sprite

    char *etiqueta;
    uint8_t ancho, alto;
    uint8_t **pixeles;
};




// Funcion propia para solucionar ciertos problemas

// (AUX)

uint16_t multiplo_de_8(uint16_t ancho){
    if(ancho % 8 == 0){
        return ancho;
    }
    else {
        return ((ancho + 7) / 8) * 8;
    }
}


// (1)

sprite_t *sprite_crear(FILE *fi){

    char etiqueta[20];  // No me parecio necesario pedir memoria

    size_t n = fread(etiqueta, sizeof(char), 20, fi);
    if(n != 20){
        return NULL;
    }

    // Asumo que si leyo esto puede leer todo

    sprite_t *sprite = malloc(sizeof(sprite_t));
    if(sprite == NULL) return NULL;

    sprite->etiqueta = malloc(20 * sizeof(char));

    memcpy(sprite->etiqueta, etiqueta, 20);

    sprite->etiqueta[19] = '\0';

    uint8_t aux[1];

    fread(aux, sizeof(uint8_t), 1, fi);

    sprite->ancho = aux[0]; 

    fread(aux, sizeof(uint8_t), 1, fi);

    uint8_t alto = aux[0]; 

    sprite->pixeles = malloc(alto * sizeof(uint8_t *));
        if(sprite->pixeles == NULL) {
            free(sprite->etiqueta);
            free(sprite);
            return NULL;
        }

    sprite->alto = 0;

    for(size_t f = 0; f < alto; f++){
        sprite->pixeles[f] = malloc((multiplo_de_8(sprite->ancho) / 8) * sizeof(uint8_t)); 
        if(sprite->pixeles[f] == NULL){
            sprite_destruir(sprite);
            return NULL;
        }
        sprite->alto++;
    }

    for(size_t f = 0; f < sprite->alto; f++){
        fread(sprite->pixeles[f], sizeof(uint8_t), multiplo_de_8(sprite->ancho) / 8, fi);
    }

    return sprite;

}


// (2)

void sprite_destruir(sprite_t *s){
    for(size_t f = 0; f < s->alto; f++){
        free(s->pixeles[f]);
    }
    free(s->etiqueta);
    free(s->pixeles);
    free(s); 
}


// (3) 

bool sprite_a_pbm(const sprite_t *s, FILE *fo){
    fputs("P1\n", fo);
    fprintf(fo, "%u %u\n", s->ancho, s->alto);



    for(size_t f = 0; f < s->alto; f++){
        for(size_t c = 0; c < multiplo_de_8(s->ancho) / 8; c++){
            
            if(c < (multiplo_de_8(s->ancho) / 8) - 1){
                for(size_t d = 0; d < 8; d++){
                fprintf(fo, "%u\n", ((s->pixeles[f][c]) & (1 << d)) >> d);
                }
            }
            else {
                size_t bits = (s->ancho % 8 == 0) ? 8 : (s->ancho % 8);

                for(size_t d = 0; d < bits; d++){
                    fprintf(fo, "%u\n", (s->pixeles[f][c] >> d) & 1);
                }
            }
        }
    }
    return true;

}


// GETTERS

// (4)

size_t sprite_ancho(const sprite_t *s){
    return s->ancho;
}


// (5)

size_t sprite_alto(const sprite_t *s){
    return s->alto;
}


// (6)

char *sprite_etiqueta(const sprite_t *s){
    return s->etiqueta;
}


// (7)

// 

bool sprite_obtener(const sprite_t *s, size_t fila, size_t col){
    
    size_t corcol = col / 8;
    size_t rescol = col % 8;
    
    return ((s->pixeles[fila][corcol] & (1 << rescol)) != 0); // Ver si da
}


// SETTER

// (8)

bool sprite_establecer(sprite_t *s, size_t fila, size_t col, bool valor){
    if(fila < s->alto && col < s->ancho){

        size_t corcol = col / 8;
        size_t rescol = col % 8;

        if(valor){
            s->pixeles[fila][corcol] |= (1 << rescol);
            return true;
        }
        else {
            s->pixeles[fila][corcol] &= ~(1 << rescol);
            return true;
        }
    }
    else {
        return false;
    }
}