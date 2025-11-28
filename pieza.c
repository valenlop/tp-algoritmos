
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
    imagen_t *img;
    color_t def;
    size_t x; //Coordenadas (x,y) -> (f,c)
    size_t y;
    size_t ancho;
    size_t alto;
};

//FUNCIONES STATIC: Ambas funciones en el main deben inicializarse con srand(time(NULL))

static e_color_t random_e() {
    e_color_t colores[4] = {E_ROJO, E_VERDE, E_AZUL, E_AMARILLO};
    return colores[rand() % 4];
}

static color_t color_random() {
    uint8_t d = 12 + (rand() % 8);
    uint8_t f = 12 + (rand() % 8);
    uint8_t e = random_e();

    return color_desde_def(d, e, f);
}

//FUNCIONES PUBLICAS

// Tendria que ver la imagen de la pieza de donde viene, podria en la misma funcion llamar a random para la forma de la pieza

pieza_t *pieza_crear(imagen_t *img, size_t x, size_t y) {
    if (img == NULL) return NULL;

    pieza_t *p = malloc(sizeof(pieza_t));
    if (p == NULL) return NULL;

    p->img = img;
    p->x = x;
    p->y = y;
    p->def = color_random(); // Cual es el objetivo de esto? , la img que recibe deberia ya tener sus colores

    p->ancho = imagen_ancho(img);
    p->alto  = imagen_alto(img);

    return p;
}

void pieza_destruir(pieza_t *p){
    if(p == NULL) return;

    imagen_destruir(p->img);
    free(p);
}

// Cuando llamamos a pieza_crear pieza->def es random cuando en realidad deberia ser el mismo que la pieza que recibe

pieza_t *pieza_clonar(const pieza_t *p) {
    if (p == NULL) return NULL;

    imagen_t *img_copia = imagen_clonar(p->img);
    if (img_copia == NULL) return NULL;

    pieza_t *nueva = pieza_crear(img_copia, p->x, p->y);
    if (nueva == NULL) {
        imagen_destruir(img_copia);
        return NULL;
    }

    nueva->def = p->def; // Algo asi podria arreglarlo, piso al valor random generado por pieza_crear

    nueva->ancho = p->ancho;
    nueva->alto  = p->alto;

    return nueva;
}

pieza_t *pieza_copiar(const pieza_t *origen, pieza_t *destino) {
    if (origen == NULL || destino == NULL) return NULL;

    imagen_t *nueva_img = imagen_clonar(origen->img);
    if (nueva_img == NULL) return NULL;

    imagen_destruir(destino->img);

    destino->img = nueva_img;
    destino->x = origen->x;
    destino->y = origen->y;
//   destino->e = origen->e;  Esto no se si estaba en la struct y se borro

    destino->ancho = origen->ancho;
    destino->alto  = origen->alto;

    return destino;
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

// Seria bueno saber que sentido tiene esto

color_t pieza_get_color(const pieza_t *p) {
    return p->def;
}

color_t pieza_color_pixel(const pieza_t *p, size_t x, size_t y) {
    return imagen_obtener_pixel(p->img, x, y);
}

size_t pieza_ancho(const pieza_t *p) {
    return p->ancho;
}

size_t pieza_alto(const pieza_t *p) {
    return p->alto;
}

void pieza_mover_x(pieza_t *p, size_t x) {
    p->x += x;
}

void pieza_mover_y(pieza_t *p, size_t y) {
    p->y += y;
}

void pieza_set_posicion(pieza_t *p, size_t x, size_t y) { // Podriamos verificar que la posicion (x,y) es factible
    p->x = x;
    p->y = y;
}


bool pieza_rotar(pieza_t *p){ 
    if (p == NULL) return false;

    imagen_t *rotada = imagen_rotar(p->img);
    if (rotada == NULL) return false;

    imagen_destruir(p->img); 
    p->img = rotada;

    p->ancho = imagen_ancho(rotada);
    p->alto  = imagen_alto(rotada);

    return true;
}
