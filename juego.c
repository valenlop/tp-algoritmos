// Implemento firmas juego.h

#include <stdlib.h>
#include <string.h>

#include "juego.h"
#include "tablero.h"


#define filas 100
#define columnas 100

typedef struct juego {
    tablero_t *tablero;

    pieza_t *pieza_actual;
    pieza_t *pieza_siguiente; // Me falta ver si es asi, tiene posicion fija. (Parece servir)

    size_t puntaje;

    double velocidad_actual; // Velocidad de caida de piezas

    bool terminado;
} juego_t;

/*
static void bajar_pieza(juego_t *j) {

    if (tablero_colisiona(j->tablero, j->pieza_actual)) {

        tablero_colocar(j->tablero, j->pieza_actual, j->pieza_x, j->pieza_y);

        unsigned lineas_nuevas = tablero_eliminar_lineas(j->tablero);
        j->lineas += lineas_nuevas;
        j->puntaje += lineas_nuevas;

        pieza_destruir(j->pieza_actual);

        j->pieza_actual = j->pieza_siguiente;
        j->pieza_siguiente = crear_pieza_random(j->sprite_formas);

        resetear_posicion_pieza(j);

        if (tablero_colisiona(j->tablero, j->pieza_actual, j->pieza_x, j->pieza_y)) {
            j->terminado = true;
        }
    }
    else {
        j->pieza_y += 1;
    }
}*/

// Inicia un juego, deberia recibir los archivos del fondo y el de los sprites

// Tiene que: Agarrar la imagen de fondo. Setear pieza y pieza siguiente. Empezar a caer la pieza_actual y pegar pieza_siguiente en el fondo

// Iniciar el tiempo en 0 (vere despues como avanza), los clears en 0 y el puntaje en 0. 

// Tambien tiene que usar "tubo" y pegarla en la parte del color siguiente. 

juego_t *juego_crear(imagen_t *fondo, sprites_t *figuras) {

    juego_t *j = malloc(sizeof(juego_t));
    if (j == NULL) return NULL;

    j->tablero = tablero_crear(columnas, filas);
    if (j->tablero == NULL) {
        free(j);
        return NULL;
    }

    j->sprite_crear(FILE *f);

    j->pieza_actual = pieza_crear(j->sprite_formas, 0, 0);
    j->pieza_siguiente = pieza_crear(j->sprite_formas, 0, 0);

    j->puntaje = 0;
    j->lineas = 0;

    j->tiempo_ms = 0;
    j->terminado = false;

    return j;
}

void juego_destruir(juego_t *j) {

    if (!j) return;

    tablero_destruir(j->tablero);

    imagen_destruir(j->sprite_formas);
    imagen_destruir(j->sprite_tubos);

    pieza_destruir(j->pieza_actual);
    pieza_destruir(j->pieza_siguiente);

    free(j);
}


size_t juego_get_puntaje(const juego_t *j) {
    return j->puntaje;
}


bool juego_terminado(const juego_t *j) {
    if (tablero_perdio(j->tablero, j->pieza_actual)) 
        j->terminado = true;
    else
        j->terminado = false;

    return j->terminado;
}