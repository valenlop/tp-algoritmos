// Revisar ((No esta terminado))


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "pieza.h"
#include "imagen.h"
#include "color.h"

typedef enum {
    E_ROJO      = 0,
    E_AMARILLO  = 11,
    E_VERDE     = 22,
    E_AZUL      = 44
} e_color_t;

struct pieza {
    // tetramino tiene la imagen del tetramino
    // fila y columna seran la posicion actual del tetramino en el tablero
    // Si tetramino == NULL entonces fila y columna no deberian existir

    imagen_t *tetramino;
    size_t fila; // 
    size_t columna;
};

//FUNCIONES STATIC: Ambas funciones en el main deben inicializarse con srand(time(NULL))

static e_color_t random_e() {
    e_color_t colores[4] = {E_ROJO, E_VERDE, E_AZUL, E_AMARILLO};
    return colores[rand() % 4];
}

static color_t color_random(e) {
    uint8_t d = 12 + (rand() % 8);
    uint8_t f = 12 + (rand() % 8);
    uint8_t e = e;

    return color_desde_def(d, e, f);
}

//FUNCIONES PUBLICAS

// Crea una pieza en base a un sprite, la pieza tiene un color random con E fijo

pieza_t *pieza_crear(sprite_t *tetramino, size_t fila, size_t columna) {
    if (tetramino == NULL) return NULL;

    pieza_t *p = malloc(sizeof(pieza_t));
    if (p == NULL) return NULL;

    p->tetramino = imagen_crear(sprite_ancho(tetramino), sprite_alto(tetramino));
    if(p->tetramino == NULL){
        free(p);
        return NULL;
    }

    e_color_t e = random_e();

    for(size_t f = 1; f <= sprite_alto(tetramino); f++){
        for(size_t c = 1; c <= sprite_ancho(tetramino); c++){
            if(! sprite_obtener(tetramino, f, c)){ // Los valores del sprite que importan parecen estar en negro por eso niego (puede fallar esto)
                imagen_setear_pixel(p->tetramino, f, c, color_random(e));
            }
        }
    }

    p->fila = fila;
    p->columna = columna;


    return p;
}

void pieza_destruir(pieza_t *p){
    if(p == NULL) return;

    imagen_destruir(p->tetramino);
    free(p);
}


const imagen_t *pieza_get_imagen(const pieza_t *p){
    return p->tetramino;
}

size_t pieza_get_fila(const pieza_t *p){
    return p->fila;
}

size_t pieza_get_columna(const pieza_t *p){
    return p->columna;
}

// No admite fila == 0 o columna == 0

color_t pieza_color_pixel(pieza_t *p, size_t fila, size_t columna) {
    return imagen_obtener_pixel(p->tetramino, fila, columna);
}

size_t pieza_ancho(pieza_t *p) {
    return imagen_ancho(p->tetramino);
}

size_t pieza_alto(pieza_t *p) {
    return imagen_alto(p->tetramino);
}

// Asumo que sera mover una cantidad x desde donde esta

void pieza_mover_fila(pieza_t *p, size_t cantidad) {
    p->fila += cantidad;
}

// Asumo que sera mover una cantidad y desde donde esta

void pieza_mover_columna(pieza_t *p, size_t cantidad) {
    p->columna += cantidad;
}

void pieza_set_posicion(pieza_t *p, size_t fila, size_t columna) { // Podriamos verificar que la posicion (x,y) es factible
    p->fila = fila;
    p->columna = columna;
}


// Tener cuidado con la posicion que puede variar al rotar la pieza

bool pieza_rotar(pieza_t *p){ 
    if (p == NULL) return false;

    imagen_t *rotada = imagen_rotar(p->tetramino);
    if (rotada == NULL) return false;

    imagen_destruir(p->tetramino); 
    p->tetramino = rotada; 

    return true;
}
