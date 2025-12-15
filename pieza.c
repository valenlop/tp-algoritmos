// Terminar: Posicion pieza relativa al tablero, offset de la pieza chica, offset del tubo y offset de los puntos


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

const char *formas[] = { "i", "l", "j", "s", "z", "t", "o" };

const char *formas_chicas[]  = { "ic","lc","jc","sc","zc","tc","oc" };

struct pieza {
    // tetramino tiene la imagen del tetramino
    // fila y columna seran la posicion actual del tetramino en el tablero
    // Si tetramino == NULL entonces fila y columna no deberian existir
    // forma es claro que es la forma de la pieza "i", "o", "s", "z", "t"...

    char *forma; 
    imagen_t *tetramino;
    size_t fila; 
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

// Crea una pieza random de casos posibles de un contenedor, la pieza tiene un color random E para pixeles validos y negro para el resto

pieza_t *pieza_crear(sprites_t *tetraminos) {
    if(tetraminos == NULL) return NULL;

    pieza_t *p = malloc(sizeof(pieza_t));
    if(p == NULL) return NULL;

    // Aca tengo que llamar a random para la forma de la pieza

    p->forma = formas[rand() % 7];

    sprite_t *tetramino = sprites_obtener(tetraminos, p->forma);

    p->tetramino = imagen_crear(sprite_ancho(tetramino), sprite_alto(tetramino));
    if(p->tetramino == NULL){
        free(p);
        return NULL;
    }

    // Aca laburo con su color

    e_color_t e = random_e();

    for(size_t f = 0; f < sprite_alto(tetramino); f++){
        for(size_t c = 0; c < sprite_ancho(tetramino); c++){
            if(! sprite_obtener(tetramino, f, c)){ // sprite_obtener es 0-based
                imagen_setear_pixel(p->tetramino, f, c, color_random(e)); // imagen_setear_pixel esta en 0-based (anda bien)
            }
            else{
                imagen_setear_pixel(p->tetramino, f, c, 0x00);
            }
        }
    }

    p->fila = 0;
    p->columna = 8 * 4; // Posiciones relativas al tablero


    return p;
}

void pieza_destruir(pieza_t *p){
    if(p == NULL) return;

    imagen_destruir(p->tetramino);
    free(p);
}


imagen_t *pieza_get_imagen(const pieza_t *p){
    return p->tetramino;
}

size_t pieza_get_fila(const pieza_t *p){
    return p->fila;
}

size_t pieza_get_columna(const pieza_t *p){
    return p->columna;
}

color_t pieza_color_pixel(pieza_t *p, size_t fila, size_t columna) {
    return imagen_obtener_pixel(p->tetramino, fila, columna);
}

size_t pieza_ancho(pieza_t *p) {
    return imagen_ancho(p->tetramino);
}

size_t pieza_alto(pieza_t *p) {
    return imagen_alto(p->tetramino);
}

// Funcion para colorear piezas (util para los numeros y pieza_siguiente)

void pieza_colorear(pieza_t *p, color_t color, bool random){
    for(size_t f = 0; f < pieza_alto(p); f++){
        for(size_t c = 0; c < pieza_ancho(p); c++){
            if(pieza_color_pixel(p, f, c)){ 
                imagen_setear_pixel(p->tetramino, f, c, color);
            }
            else {
                imagen_setear_pixel(p->tetramino, f, c, 0x00);
            }
        }
    }
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

// Para saber la forma de la pieza (la dejo pero creo que no sirve)

char *pieza_forma(pieza_t *p){
    return p->forma;
}

pieza_t *pieza_a_version_chica(pieza_t *p, sprites_t *ss){
    pieza_t *pieza_chica = malloc(sizeof(pieza_t));
    if(pieza_chica == NULL) return NULL;

    pieza_chica->forma = malloc(sizeof(char) * 3);
    if(p->forma == NULL){
        free(pieza_chica);
        return NULL;
    }

    pieza_chica->forma = forma_chica_desde_grande(p->forma);
    
    sprite_t *s_chico = sprites_obtener(ss, pieza_chica->forma);

    pieza_chica->tetramino = imagen_crear(sprite_ancho(s_chico), sprite_alto(s_chico)); // Tal vez deba chequear que pudo

    pieza_colorear(pieza_chica, 0xff, false); // Coloreo de blanco sin random

    pieza_chica->fila = ; // Faltan los offset de esta pieza

    pieza_chica->columna = ;

    return pieza_chica;

}

// Matcheo entre version grande y chica

static const char *forma_chica_desde_grande(const char *g) {
    for (size_t i = 0; i < 7; i++){
        if(strcmp(formas[i], g) == 0)
            return formas_chicas[i];
    }
    return NULL;
}

pieza_t *pieza_crear_tubo(pieza_t *p, sprites_t *ss){
    
    sprite_t *s_tubo = sprites_obtener(ss, "tubo");

    pieza_t *tubo = malloc(sizeof(pieza_t));
    if(tubo == NULL) return NULL;

    tubo->tetramino = imagen_crear(sprite_ancho(s_tubo), sprite_alto(s_tubo));
    if(tubo->tetramino == NULL){
        free(tubo);
        return NULL;
    }


    
    // Aca laburo con su color

    e_color_t e = color_e(imagen_obtener_pixel(p->tetramino, 1, 1));

    for(size_t f = 0; f < sprite_alto(s_tubo); f++){
        for(size_t c = 0; c < sprite_ancho(tubo); c++){
            if(! sprite_obtener(s_tubo, f, c)){ // sprite_obtener es 0-based
                imagen_setear_pixel(p->tetramino, f, c, color_random(e)); // imagen_setear_pixel esta en 0-based (anda bien)
            }
            else{
                imagen_setear_pixel(p->tetramino, f, c, 0x00);
            }
        }
    }

    tubo->fila = ; // Faltan los offset del tubo
    tubo->columna = ;
    tubo->forma = NULL;

    return tubo;
}

pieza_t *pieza_crear_dos_puntos(sprites_t *ss){
    
    sprite_t *s_dos_puntos = sprites_obtener(ss, ":");

    pieza_t *dos_puntos = malloc(sizeof(pieza_t));
    if(dos_puntos == NULL) return NULL;

    dos_puntos->tetramino = imagen_crear(sprite_ancho(s_dos_puntos), sprite_alto(s_dos_puntos));
    if(dos_puntos->tetramino == NULL){
        free(dos_puntos);
        return NULL;
    }


    
    // Aca laburo con su color

    for(size_t f = 0; f < sprite_alto(s_dos_puntos); f++){
        for(size_t c = 0; c < sprite_ancho(dos_puntos); c++){
            if(! sprite_obtener(s_dos_puntos, f, c)){ // sprite_obtener es 0-based
                imagen_setear_pixel(dos_puntos->tetramino, f, c, 0xff); // Color blanco
            }
            else{
                imagen_setear_pixel(dos_puntos->tetramino, f, c, 0x00);
            }
        }
    }

    dos_puntos->fila = ; // Faltan los offset de los dos puntos
    dos_puntos->columna = ;
    dos_puntos->forma = NULL;

    return dos_puntos;
}
