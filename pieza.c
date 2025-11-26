// Hay que revisarlo ((Las firmas no coinciden entre el .c y el .h))

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "imagen.h"
#include "color.h"

typedef struct pieza {
    imagen_t *img;
    size_t x, y;
    char tipo;    // Preguntar como es que funciona el tipo
    color_t e;
} pieza_t;

// Cuando creamos la pieza, su imagen debe tener la componente de E que recibe pero el resto debe llamar a random

pieza_t *pieza_crear(imagen_t *img, size_t x, size_t y, char tipo, uint8_t e){
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



const imagen_t *pieza_get_imagen(const pieza_t *p){
    return p->img;
}

size_t pieza_get_x(const pieza_t *p){
    return p->x;
}

size_t pieza_get_y(const pieza_t *p){
    return p->y;
}

char pieza_get_tipo(const pieza_t *p){
    return p->tipo;
}

uint8_t pieza_get_e(const pieza_t *p){
    return p->e;
}



color_t pieza_color_pixel(pieza_t *p, size_t x, size_t y){ // Falta implementar esto (lo necesito para tablero.h)

}

size_t pieza_ancho(pieza_t *p){ // Falta implementar esto (lo necesito para tablero.h)

}

size_t pieza_alto(pieza_t *p){ // Falta implementar esto (lo necesito para tablero.h)

}



void pieza_mover(pieza_t *p, size_t x, size_t y){ // Faltan  chequeos de si se puede mover, deberia devolver bool
    p->x += x;
    p->y += y; 
}

void pieza_set_posicion(pieza_t *p, size_t x, size_t y){
    p->x = x;
    p->y = y;
}

bool pieza_rotar(pieza_t *p){ // Si no me equivoco la posicion en x y en y podria cambiar, habria que actualizar esos valores, los tetraminos no son cuadrados
    if (p == NULL) return false;

    imagen_t *img_rotada = imagen_rotar(p->img);
    if (img_rotada == NULL) return false;

    imagen_destruir(p->img);
    p->img = img_rotada;

    return true;
}

// genera numeros entre 0 y a - 1 (no se si sirve)

// Inicializa la semilla solo la primera vez que se llama
static int rand_a(int a) {
    static int seeded = 0;

    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }

    return rand() % a;   // número entre 0 y a-1
}