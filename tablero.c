// Implementacion firmas de tablero.h

#include "tablero.h"

#include <stddef.h>

struct tablero {
    imagen_t *tablero; // Parece que solo agregamos un puntero mas
};

// (1)

tablero_t *tablero_crear(size_t columnas, size_t filas){
    tablero_t *tablero_nuevo = malloc(sizeof(tablero_t));
    if(tablero_nuevo == NULL){
        return NULL;
    }

    tablero_nuevo->tablero = imagen_crear(columnas, filas); 
    if(tablero_nuevo->tablero == NULL){
        free(tablero_nuevo);
        return NULL;
    }

    imagen_iniciar_color(tablero_nuevo->tablero, 0x00); // Le paso el color negro
}

// (2)

void tablero_destruir(tablero_t *t){
    if(t == NULL) return;

    imagen_destruir(t->tablero);
    free(t);
}

// (3) Deberia ser asi de basico, pero podria faltar algo

bool tablero_perdio(tablero_t *t, pieza_t *p){
    if(tablero_colision(t, p) && pieza_get_y(p) == 0){
        return true;
    }

    else {
        return false;
    }
}

// (4) Me fijo en cada pixel de color si abajo tiene arena, puede fallar en temas de logica (faltaria ver que onda si choca con el piso)

bool tablero_colision(tablero_t *t, pieza_t *p){ // x = columna e y = fila,  Del TDA pieza_h puedo saber la posicion de p

    size_t ubicacion_x = pieza_get_x(p);
    size_t ubicacion_y = pieza_get_y(p);

    for(size_t f = ubicacion_y; f < pieza_alto(p) + ubicacion_y; f++){
        for(size_t c = ubicacion_x; c < pieza_ancho(p) + ubicacion_x; c++){
            if(pieza_color_pixel(p, c - ubicacion_x, f - ubicacion_y) != 0 && imagen_obtener_pixel(t->tablero, f + 1, c) != 0){
                return true;
            }
        }
    }

    return false;
}

// (5)

bool tablero_pegar_pieza(tablero_t *t, pieza_t *p){

}

// (6)

void tablero_simular_arena(tablero_t *t){

}

// (7)

size_t tablero_linea_formada(tablero_t *t){

}