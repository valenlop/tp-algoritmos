// Hay que revisarlo ((Las firmas no coinciden entre el .c y el .h))

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "imagen.h"
#include "color.h"

typedef struct pieza {
    imagen_t *img;
    uint8_t x, y;
    char tipo;
    color_t e;
} pieza_t;

pieza_t *pieza_crear(imagen_t *img, uint8_t x, uint8_t y, char tipo, uint8_t e){
    pieza_t *p = malloc(sizeof(pieza_t));
    if(p == NULL) return NULL;

    p->img = img;
    p->x = x;
    p->y = y;
    p->tipo = tipo;
    p->e = e;
    return p;
}

void pieza_destruir(pieza_t *p){
    if(p == NULL) return;

    imagen_destruir(p->img);
    free(p);
}

pieza_t *pieza_clonar(const pieza_t *p){
    if(p == NULL) return NULL;

    imagen_t *img_copia = imagen_clonar(p->img);
    if(img_copia == NULL) return NULL;

    return pieza_crear(img_copia, p->x, p->y, p->tipo, p->e);
}

void pieza_set_posicion(pieza_t *p, int x, int y){
    p->x = x;
    p->y = y;
}

uint8_t pieza_get_e(const pieza_t *p){
    return p->e;
}

uint8_t pieza_get_x(const pieza_t *p){
    return p->x;
}

uint8_t pieza_get_y(const pieza_t *p){
    return p->y;
}

char pieza_get_tipo(const pieza_t *p){
    return p->tipo;
}

const imagen_t *pieza_get_imagen(const pieza_t *p){
    return p->img;
}

void pieza_mover(pieza_t *p, int x, int y){
    p->x += x;
    p->y += y; 
}

bool pieza_rotar(pieza_t *p){
    if (p == NULL) return false;

    imagen_t *img_rotada = imagen_rotar(p->img);
    if (img_rotada == NULL) return false;

    imagen_destruir(p->img);
    p->img = img_rotada;

    return true;
}

void def_random(){
    
}