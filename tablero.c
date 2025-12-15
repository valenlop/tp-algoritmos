// Revisar ((Va a andar mal simular_arena y linea_formada))

// Implementacion firmas de tablero.h

#include "tablero.h"

#include <stdio.h>

struct tablero {
    // tablero es la imagen que representara al tablero del juego
    // Si tablero == NULL (no pasara nunca)

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

    return tablero_nuevo;
}

// (2)

void tablero_destruir(tablero_t *t){
    if(t == NULL) return;

    imagen_destruir(t->tablero);
    free(t);
}

// (3) Deberia ser asi de basico, pero podria faltar algo (Ojo con donde es el cero de la pieza)

bool tablero_perdio(tablero_t *t, pieza_t *p){
    if(tablero_colision(t, p) && pieza_get_columna(p) == 0){
        return true;
    }

    else {
        return false;
    }
}

// (4) Me fijo en cada pixel de color si abajo tiene arena, puede fallar en temas de logica

bool tablero_colision(tablero_t *t, pieza_t *p){

    size_t columna = pieza_get_columna(p);
    size_t fila = pieza_get_fila(p);

    if(fila + pieza_alto(p) == tablero_alto(t)){
        return true;
    }

    for(size_t f = fila; f < pieza_alto(p) + fila; f++){
        for(size_t c = columna; c < pieza_ancho(p) + columna; c++){
            if(pieza_color_pixel(p, f - fila + 1, c - columna + 1) != 0 && imagen_obtener_pixel(t->tablero, f + 1, c) != 0){
                return true;
            }
        }
    }

    return false;
}

// (5) No voy a chequear que este dentro del tablero (eso deberia suceder). Lo que pase con la pieza despues se ve afuera (hablando de liberar memoria)

// Se debe asegurar que la pieza en su imagen los pixeles no participantes del tetramino son negros (0x00)

bool tablero_pegar_pieza(tablero_t *t, pieza_t *p){
    
    return imagen_pegar_no_negros(t->tablero, pieza_get_imagen(p), pieza_get_fila(p), pieza_get_columna(p)); // Ojo aca con pegar los que no son
}

// (6) Tiene algunas situaciones particulares donde se pisan los pixeles que se mueven (solucionar mas adelante). 

// Tendria que fijarse en el tablero nuevo si realmente pueden ir a la ubicacion (los pixeles)

void tablero_simular_arena(tablero_t *t){

    size_t alto = tablero_alto(t);

    size_t ancho = tablero_ancho(t);

    tablero_t *tablero_nuevo = tablero_crear(ancho, alto);
    if(tablero_nuevo == NULL) return;

    // Deberia andar 

    for(size_t c = 1; c <= ancho; c++){
        if(!imagen_setear_pixel(tablero_nuevo->tablero, alto, c, imagen_obtener_pixel(t->tablero, alto, c))){
            return;
        }
    }

    // Recorro todos los pixeles

    for(size_t f = 1; f < alto - 1; f++){
        for(size_t c = 1; c <= ancho; c++){

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

                    if(color_abajo == 0){
                        if(!imagen_setear_pixel(tablero_nuevo->tablero, f, c, color)){
                        tablero_destruir(tablero_nuevo);
                        return;
                        }
                    }
                    else if(c == 1 && color_abajo != 0){
                        color_t color_abajo_derecha = imagen_obtener_pixel(t->tablero, f + 1, c + 1);
                        if(color_abajo_derecha == 0){
                            if(!imagen_setear_pixel(tablero_nuevo->tablero, f, c, color)){
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
                    else if(c == ancho && color_abajo != 0){
                        color_t color_abajo_izquierda = imagen_obtener_pixel(t->tablero, f + 1, c - 1);

                        if(color_abajo_izquierda == 0){
                            if(!imagen_setear_pixel(tablero_nuevo->tablero, f, c, color)){
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
                    else{

                        color_t color_abajo_izquierda = imagen_obtener_pixel(t->tablero, f + 1, c - 1);
                        color_t color_abajo_derecha = imagen_obtener_pixel(t->tablero, f + 1, c + 1);

                        if(color_abajo_derecha == 0 && color_abajo_izquierda == 0){
                            if(rand() % 2 == 1){
                                if(!imagen_setear_pixel(tablero_nuevo->tablero, f, c, color)){
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
                        else if(color_abajo_izquierda == 0 && color_abajo_derecha != 0){
                            if(!imagen_setear_pixel(tablero_nuevo->tablero, f, c, color)){
                                tablero_destruir(tablero_nuevo);
                                return;
                            }
                        }
                        else if(color_abajo_izquierda != 0 && color_abajo_derecha == 0){
                            if(!imagen_setear_pixel(tablero_nuevo->tablero, f, c, color)){
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
    imagen_t *visitados = imagen_crear(tablero_ancho(t), tablero_alto(t));
    if(visitados == NULL){
        printf("Fallo pedido de memoria");
        return false;
    }

    for(size_t f = 1; f <= tablero_alto(t); f++){
        color_t color_buscado = imagen_obtener_pixel(t->tablero, f, 1);

        if(color_buscado){
            if(visitar(t, visitados, f, 1, color_buscado)){
                *puntaje = tablero_bloque_eliminar(t, visitados);
                imagen_destruir(visitados);
                return true;
            }
        }
    }
    imagen_destruir(visitados);
    return false;


}

// Esta es la que visita cada pixel

static bool visitar(tablero_t *t, imagen_t *visitados, size_t f, size_t c, color_t color_buscado){

    size_t ancho = tablero_ancho(t);

    // (1) Chequear que f y c esten en rango (se pueden escapar del tablero)

    if(f < 1 || f > tablero_alto(t) || c < 1 || c > ancho){
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

    // (4) Chequeo si llegue a la ultima columna (exploro hacia arriba y abajo)

    if(c == ancho){
        imagen_setear_pixel(visitados, f, c, 1);

        visitar(t, visitados, f + 1, c, color_buscado);
        visitar(t, visitados, f - 1, c, color_buscado);

        return true;
    }

    // (5) Marcar el pixel como visitado en visitados (en este caso el pixel es del color pero no llegamos al final)

    imagen_setear_pixel(visitados, f, c, 1);

    // (6) Ir hacia derecha, arriba, abajo e izquiera (en ese orden)

    return visitar(t, visitados, f, c + 1, color_buscado)|| // Derecha
    visitar(t, visitados, f - 1, c, color_buscado) || // Arriba
    visitar(t, visitados, f + 1, c, color_buscado) || // Abajo
    visitar(t, visitados, f, c - 1, color_buscado); // Izquierda

}

// Podria usar la matriz de visitados y eliminar esos pixeles

static size_t tablero_bloque_eliminar(tablero_t *t, imagen_t *visitados){
    size_t ancho = tablero_ancho(t);
    size_t alto = tablero_alto(t);
    size_t puntaje = 0;

    for(size_t f = 1; f <= alto; f++){
        for(size_t c = 1; c <= ancho; c++){
            if(imagen_obtener_pixel(visitados, f, c)){
                imagen_setear_pixel(t->tablero, f, c, 0);
                puntaje++;
            }
        }
    }

    return puntaje;

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