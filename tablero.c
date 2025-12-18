
// Implementacion firmas de tablero.h

#include "tablero.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct tablero {
    // tablero es la imagen que representara al tablero del juego
    // Si tablero == NULL (no pasara nunca)

    imagen_t *tablero;
};

// Esta es la que visita cada pixel

static bool visitar(tablero_t *t, imagen_t *visitados, size_t f, size_t c, color_t color_buscado){

    size_t ancho = tablero_ancho(t);

    // (1) Chequear que f y c esten en rango (se pueden escapar del tablero)

    if(f >= tablero_alto(t) || c >= ancho){
        return false;
    }

    // (2) Verificar si fue visitado (usar visitados)

    if(imagen_obtener_pixel(visitados, f, c) != 0){
        return false;
    }

    // (3) Chequeo si el pixel tiene distinto color que color_busqueda (parte del color E)

    if(color_e(color_buscado) != color_e(imagen_obtener_pixel(t->tablero, f, c))){
        return false;
    }

    // Chequeo de pixel negro (necesario porque la parte E del rojo y el negro es la misma)

    if(imagen_obtener_pixel(t->tablero, f , c) == 0){
        return false;
    }

    // (4) Chequeo si llegue a la ultima columna (exploro hacia arriba y abajo)

    if(c == ancho - 1){
        imagen_setear_pixel(visitados, f, c, 1);
        return true;
    }

    // (5) Marcar el pixel como visitado en visitados (en este caso el pixel es del color pero no llegamos al final)

    imagen_setear_pixel(visitados, f, c, 0xff); // Pinto de blanco visitados

    // (6) Ir hacia derecha, arriba, abajo e izquiera (en ese orden)

    return visitar(t, visitados, f, c + 1, color_buscado)|| // Derecha
    visitar(t, visitados, f - 1, c, color_buscado) || // Arriba
    visitar(t, visitados, f + 1, c, color_buscado) || // Abajo
    visitar(t, visitados, f, c - 1, color_buscado); // Izquierda

}

static void completar_visita(tablero_t *t, imagen_t *visitados, size_t f, size_t c, color_t color_buscado){
    size_t ancho = imagen_ancho(visitados);
    size_t alto  = imagen_alto(visitados);

    if(f >= alto || c >= ancho)
        return;

    if(imagen_obtener_pixel(visitados, f, c) != 0)
        return;

    color_t color_actual = imagen_obtener_pixel(t->tablero, f, c);
    if(color_actual == 0)
        return;

    if(color_e(color_actual) != color_e(color_buscado))
        return;

    imagen_setear_pixel(visitados, f, c, 0xff);

    if(f > 0)
        completar_visita(t, visitados, f - 1, c, color_buscado);
    if(f + 1 < alto)
        completar_visita(t, visitados, f + 1, c, color_buscado);
    if(c > 0)
        completar_visita(t, visitados, f, c - 1, color_buscado);
    if(c + 1 < ancho)
        completar_visita(t, visitados, f, c + 1, color_buscado);
}



// Podria usar la matriz de visitados y eliminar esos pixeles

static size_t tablero_bloque_eliminar(tablero_t *t, imagen_t *visitados){
    size_t ancho = tablero_ancho(t);
    size_t alto = tablero_alto(t);
    size_t puntaje = 0;

    for(size_t f = 0; f < alto; f++){
        for(size_t c = 0; c < ancho; c++){
            if(imagen_obtener_pixel(visitados, f, c)){
                imagen_setear_pixel(t->tablero, f, c, 0x00);
                puntaje++;
            }
        }
    }

    return puntaje;

}

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

    return tablero_nuevo;
}

// (2)

void tablero_destruir(tablero_t *t){
    if(t == NULL) return;

    imagen_destruir(t->tablero);
    free(t);
}

// (3)

bool tablero_perdio(tablero_t *t, pieza_t *p){
    if(tablero_colision(t, p) && pieza_get_fila(p) == 0){
        return true;
    }

    else {
        return false;
    }
}

// (4)

bool tablero_colision(tablero_t *t, pieza_t *p){

    size_t columna = pieza_get_columna(p);
    size_t fila = pieza_get_fila(p);

    if(fila + pieza_alto(p) >= tablero_alto(t)){
        return true;
    }

    for(size_t f = 0; f < pieza_alto(p); f++){
        for(size_t c = 0; c < pieza_ancho(p); c++){
            if (pieza_color_pixel(p, f, c) == 0)
                continue;

            size_t tf = fila + f + 1; // pixel debajo
            size_t tc = columna + c;

            // chequeo defensivo (opcional pero sano)
            if (tf >= tablero_alto(t))
                return true;

            if (imagen_obtener_pixel(t->tablero, tf, tc) != 0)
                return true;
        }
    }

    return false;
}

// (5) 

bool tablero_pegar_pieza(tablero_t *t, pieza_t *p){
    
    return imagen_pegar_no_negros(t->tablero, pieza_get_imagen(p), pieza_get_fila(p), pieza_get_columna(p));
}

// (6) 

void tablero_simular_arena(tablero_t *t){

    size_t alto = tablero_alto(t);

    size_t ancho = tablero_ancho(t);

    tablero_t *tablero_nuevo = tablero_crear(ancho, alto);
    if(tablero_nuevo == NULL) return;

    // Deberia andar 

    for(size_t c = 0; c < ancho; c++){
        if(!imagen_setear_pixel(tablero_nuevo->tablero, alto - 1, c, imagen_obtener_pixel(t->tablero, alto - 1, c))){
            tablero_destruir(tablero_nuevo);
            return;
        }
    }

    // Recorro todos los pixeles, no piso pixeles de color con otros de color (lo doy por terminado)

    for(size_t f = alto - 2; f < alto - 1 ; f--){
        for(size_t c = 0; c < ancho; c++){

            color_t color = imagen_obtener_pixel(t->tablero, f, c);

            if(color){
                if(rand() % 4 != 0){
                    if(!imagen_setear_pixel(tablero_nuevo->tablero, f, c, color)){
                        tablero_destruir(tablero_nuevo);
                        return;
                    }
                }
                else{

                    color_t color_abajo = imagen_obtener_pixel(t->tablero, f + 1, c);
                    color_t color_abajo_t_nuevo = imagen_obtener_pixel(tablero_nuevo->tablero, f + 1, c); // Esto va a complicar la cosa pero simula bien

                    if(color_abajo == 0 && color_abajo_t_nuevo == 0){ // Se puede bajar y no pisa otro pixel de color (cualquier columna)
                        if(!imagen_setear_pixel(tablero_nuevo->tablero, f + 1, c, color)){
                            tablero_destruir(tablero_nuevo);
                            return;
                        }
                    }
                    else if(c == 0){ // Primer columna
                        color_t color_abajo_derecha = imagen_obtener_pixel(t->tablero, f + 1, c + 1);
                        color_t color_abajo_derecha_t_nuevo = imagen_obtener_pixel(tablero_nuevo->tablero, f + 1, c + 1);

                        if(color_abajo_derecha == 0 && color_abajo_derecha_t_nuevo == 0){
                            if(!imagen_setear_pixel(tablero_nuevo->tablero, f + 1, c + 1, color)){
                                tablero_destruir(tablero_nuevo);
                                return;
                            }
                        }
                        else{
                            if(!imagen_setear_pixel(tablero_nuevo->tablero, f, c, color)){
                                tablero_destruir(tablero_nuevo);
                                return;
                            }
                        }
                    }
                    else if(c == ancho - 1){ // Ultima columna
                        color_t color_abajo_izquierda = imagen_obtener_pixel(t->tablero, f + 1, c - 1);
                        color_t color_abajo_izquierda_t_nuevo = imagen_obtener_pixel(tablero_nuevo->tablero, f + 1, c - 1);

                        if(color_abajo_izquierda == 0 && color_abajo_izquierda_t_nuevo == 0){
                            if(!imagen_setear_pixel(tablero_nuevo->tablero, f + 1, c - 1, color)){
                                tablero_destruir(tablero_nuevo);
                                return;
                            }
                        }
                        else{
                            if(!imagen_setear_pixel(tablero_nuevo->tablero, f, c, color)){
                                tablero_destruir(tablero_nuevo);
                                return;
                            }
                        }
                    }
                    else{ // Todas las columnas intermedias y no se puede bajar (solo en diagonal)
                        assert(c < ancho - 1);

                        color_t color_abajo_izquierda = imagen_obtener_pixel(t->tablero, f + 1, c - 1);
                        color_t color_abajo_derecha = imagen_obtener_pixel(t->tablero, f + 1, c + 1);

                        color_t c_abajo_i_t_nuevo = imagen_obtener_pixel(tablero_nuevo->tablero, f + 1, c - 1);
                        color_t c_abajo_d_t_nuevo = imagen_obtener_pixel(tablero_nuevo->tablero, f + 1, c + 1);

                        if(color_abajo_derecha == 0 && color_abajo_izquierda == 0 && c_abajo_i_t_nuevo == 0 && c_abajo_d_t_nuevo == 0){
                            if(rand() % 2 == 1){
                                if(!imagen_setear_pixel(tablero_nuevo->tablero, f + 1, c - 1, color)){ // Pego a la izquierda y no piso pixel de color
                                    tablero_destruir(tablero_nuevo);
                                    return;
                                }
                            }
                            else{
                                if(!imagen_setear_pixel(tablero_nuevo->tablero, f + 1, c + 1, color)){ // Pego a la derecha y no piso pixel de color
                                    tablero_destruir(tablero_nuevo);
                                    return;
                                }
                            }
                        }
                        else if(color_abajo_izquierda == 0 && color_abajo_derecha != 0 && c_abajo_i_t_nuevo == 0){
                            if(!imagen_setear_pixel(tablero_nuevo->tablero, f + 1, c - 1, color)){ // Pego a la izquiera (unico caso)
                                tablero_destruir(tablero_nuevo);
                                return;
                            }
                        }
                        else if(color_abajo_izquierda != 0 && color_abajo_derecha == 0 && c_abajo_d_t_nuevo == 0){
                            if(!imagen_setear_pixel(tablero_nuevo->tablero, f + 1, c + 1, color)){ // Pego a la derecha (unico caso)
                                tablero_destruir(tablero_nuevo);
                                return;
                            }
                        }
                        else{
                            if(!imagen_setear_pixel(tablero_nuevo->tablero, f, c, color)){
                                tablero_destruir(tablero_nuevo);
                                return;
                            }
                        }
                    }
                }
            }
        }   
    }

    imagen_destruir(t->tablero); // Libero imagen vieja

    t->tablero = tablero_nuevo->tablero; // Apunto mi tablero nuevo al viejo

    free(tablero_nuevo); // Libero el puntero pedido que ya no me sirve
}

// (7) Puede faltar algo pero deberia andar

bool tablero_linea_formada(tablero_t *t, size_t *puntaje){

    size_t alto = tablero_alto(t);

    // Creo superimagen de visitados

    imagen_t **superimagen = calloc(alto, sizeof(imagen_t *));
    if(superimagen == NULL) return false;

    for(size_t f = 0; f < alto; f++){
        color_t color_buscado = imagen_obtener_pixel(t->tablero, f, 0);

        if(color_buscado){
            superimagen[f] = imagen_crear(tablero_ancho(t), tablero_alto(t));
            if(superimagen[f] == NULL){
                for(size_t j = 0; j < f; j++){
                    if(superimagen[j]){
                        imagen_destruir(superimagen[j]);
                    }
                }
                free(superimagen);
                printf("Fallo pedido de memoria");
                return false;
            }

            imagen_iniciar_color(superimagen[f], 0x00);

            if(visitar(t, superimagen[f], f, 0, color_buscado)){
                completar_visita(t, superimagen[f], f, 0, color_buscado); // Tengo la linea formada, me falta el bloque completo
                *puntaje = tablero_bloque_eliminar(t, superimagen[f]);
                for(size_t j = 0; j < f; j++){
                    if(superimagen[j]){
                        imagen_destruir(superimagen[j]);
                    }
                }
                free(superimagen);
                return true;
            }
        }
    }
    for(size_t j = 0; j < alto; j++){
        if(superimagen[j]){
            imagen_destruir(superimagen[j]);
        }
    }
    free(superimagen);
    return false;


}


// (8)

size_t tablero_alto(tablero_t *t){
    return imagen_alto(t->tablero);
}

// (9)

size_t tablero_ancho(tablero_t *t){
    return imagen_ancho(t->tablero);
}


imagen_t *tablero_imagen(tablero_t *t){
    return t->tablero;
}