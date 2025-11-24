// Implementacion firmas de tablero.h

#include "tablero.h"

#include <stddef.h>

struct tablero {
    imagen_t *tablero; // Parece que solo agregamos un puntero mas
};

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

bool tablero_colision(tablero_t *t, pieza_t *p, size_t x, size_t y); // x = columna e y = fila

size_t tablero_linea_formada(tablero_t *t){

}