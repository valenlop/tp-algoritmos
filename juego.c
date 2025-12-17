// Terminar: Offset puntaje, clears, minutos, segundos y milisegundos

#include <stdlib.h>
#include <string.h>

#include "juego.h"


#define filas 144 // Deberian ser estos los valores
#define columnas 80

struct juego {
    imagen_t *fondo;

    imagen_t *frame; // Frame variable (PPM que tengo al inicio)

    tablero_t *tablero; // Tablero del juego

    pieza_t *pieza_actual; // Figura de la pieza actual

    pieza_t *pieza_siguiente; // Figura de la pieza siguiente (Ambas piezas son asignables)

    pieza_t *tubo; // El tubo con el color de la pieza siguiente, es una pieza fija en ubicacion, todo el tiempo que existe se genera una pieza del color

    size_t puntaje; // Puntaje del juego (cantidad de los píxeles eliminados hasta el momento)

    size_t clears; // Cantidad de clears hechos hasta el momento

    double velocidad_actual; // Velocidad de caida de piezas

    uint32_t minutos, segundos, milisegundos; // Tiempo transcurrido desde el inicio del juego

    bool terminado; // Asumo servira si para saber si perdio y cerrar la pantalla

};

const char *numeros[] = {
    "0", "1", "2", "3", "4",
    "5", "6", "7", "8", "9"
};

// Me agarra los numeros y los pone en base a etiquetas

static const char **digito_a_texto(size_t n, int *cantidad){
    size_t aux = n;
    int cant = 0;

    if(n == 0){
        const char **etiquetas = malloc(sizeof(char *));
        if(etiquetas == NULL) return NULL;

        etiquetas[0] = numeros[0];
        *cantidad = 1;
        return etiquetas;
    }

    while(aux != 0){
        cant++;
        aux /= 10;
    }

    const char **etiquetas = malloc(cant * sizeof(char *));
    if(etiquetas == NULL) return NULL;

    for(int i = cant - 1; i >=  0; i--){
        size_t resto = n % 10;
        etiquetas[i] = numeros[resto];
        n /= 10;
    }

    *cantidad = cant;
    return etiquetas;

}


// Agarra las etiquetas y genera las imagenes correspondientes (usa sprites_obtener, imagen_crear e imagen_iniciar_color)

static imagen_t **etiquetas_a_figuras(const char **etiquetas, int cantidad, sprites_t *ss){
    imagen_t **im_numeros = malloc(sizeof(imagen_t *) * cantidad);
    if(im_numeros == NULL) return NULL;
    
    for(size_t i = 0; i < cantidad; i++){

        sprite_t *s_numero = sprites_obtener(ss, etiquetas[i]);
        if(s_numero == NULL){
            for(size_t j = 0; j < i; j++)
                imagen_destruir(im_numeros[j]);
            free(im_numeros);
            return NULL;
        }

        im_numeros[i] = imagen_crear(sprite_ancho(s_numero), sprite_alto(s_numero));
        if (im_numeros[i] == NULL){
            for (size_t j = 0; j < i; j++)
                imagen_destruir(im_numeros[j]);
            free(im_numeros);
            return NULL;
        }

        for(size_t f = 0; f < sprite_alto(s_numero); f++){
            for(size_t c = 0; c < sprite_ancho(s_numero); c++){

                if(sprite_obtener(s_numero, f, c)){ 
                    imagen_setear_pixel(im_numeros[i], f, c, 0xff);
                }
                else{
                    imagen_setear_pixel(im_numeros[i], f, c, 0x00); 
                }
            }
        }
    }

    return im_numeros;
}

// Wrapper para usar en juego_generar_frame

static imagen_t **digito_a_imagen(size_t valor, int *cantidad, sprites_t *ss){
    int cant = 0;

    const char **etiquetas = digito_a_texto(valor, &cant);
    if(etiquetas == NULL) return NULL;

    imagen_t **digitos_empiezados = etiquetas_a_figuras(etiquetas, cant, ss);
    if(digitos_empiezados == NULL){
        free(etiquetas);
        return NULL;
    }

    // A esta altura ya deberia liberar lo de digito_a_texto

    free(etiquetas); // Con esto deberia estar, cada etiqueta es variable global cte

    *cantidad = cant;

    return digitos_empiezados;
}


// Inicia un juego con sus parametros (deberia ir accediendo a todos estos parametros para dibujar bien el fondo)

juego_t *juego_crear(imagen_t *fondo, sprites_t *figuras) {

    juego_t *j = malloc(sizeof(juego_t));
    if (j == NULL) return NULL;

    j->frame = imagen_crear(imagen_ancho(fondo), imagen_alto(fondo));
    if(j->frame == NULL){
        free(j);
        return NULL;
    }
    j->fondo = fondo;

    imagen_pegar(j->frame, j->fondo, 0, 0);

    j->tablero = tablero_crear(columnas, filas); // El valor de columnas y filas se fija arriba con un define 
    if(j->tablero == NULL) {
        free(j->frame);
        free(j);
        return NULL;
    }

    // Parte piezas, ambas piezas son iguales

    j->pieza_actual = pieza_crear(figuras);

    j->pieza_siguiente = pieza_crear(figuras); // Posicion igual al inicio de la pieza_actual


    // En esta parte creo la pieza tubo (debe tener el mismo color de la pieza_siguiente)

    j->tubo = NULL;

    j->puntaje = 0;

    j->clears = 0;

    j->velocidad_actual = 25; // 25 pixeles por segundo, sirve para hacer una cuenta posterior sobre la caida de las piezas

    j->minutos = j->segundos = j->milisegundos = 0; // Inician en 0 deberiar ir actualizandose

    j->terminado = false;

    return j;
}

// Esta funcion ira al final del main

void juego_destruir(juego_t *j) {

    imagen_destruir(j->frame);

    imagen_destruir(j->fondo);

    tablero_destruir(j->tablero);

    pieza_destruir(j->pieza_actual);

    pieza_destruir(j->pieza_siguiente);

    pieza_destruir(j->tubo);

    free(j);
}

// Terminada

imagen_t *juego_obtener_frame(juego_t *j){
    return j->frame;
}

// Terminada

tablero_t *juego_obtener_tablero(juego_t *j){
    return j->tablero;
}

// Terminada

pieza_t *juego_obtener_pieza_actual(juego_t *j){
    return j->pieza_actual;
}

// Terminada

pieza_t *juego_obtener_pieza_siguiente(juego_t *j){
    return j->pieza_siguiente;
}

// Podria agregar juego_obtener_tubo pero creo que no sirve para nada

// Terminada

size_t juego_obtener_puntaje(juego_t *j){
    return j->puntaje;
}

// Terminada

size_t juego_obtener_clears(juego_t *j){
    return j->clears;
}

// Terminada

double juego_obtener_velocidad_actual(juego_t *j){
    return j->velocidad_actual;
}

// Terminada

uint32_t juego_obtener_minutos(juego_t *j){
    return j->minutos;
}

// Terminada

uint32_t juego_obtener_segundos(juego_t *j){
    return j->segundos;
}

// Terminada

uint32_t juego_obtener_milisegundos(juego_t *j){
    return j->milisegundos;
}


// Termianda (deberia ser asi)

bool juego_terminado(juego_t *j) {
    if(tablero_perdio(j->tablero, j->pieza_actual)) 
        j->terminado = true;

    return j->terminado;
}

// Terminada

void juego_setear_pieza_actual(juego_t *j, pieza_t *pieza_siguiente_real){
    pieza_destruir(j->pieza_actual);
    j->pieza_actual = pieza_siguiente_real;
    j->pieza_siguiente = NULL; // Apunto a NULL, esto obliga a generar una pieza nueva
}

// 

void juego_setar_pieza_siguiente(juego_t *j, pieza_t *pieza_creada){
    j->pieza_siguiente = pieza_creada;
}

//

void juego_aumentar_nro_clears(juego_t *j){
    j->clears++;
}

//

void juego_aumentar_puntaje(juego_t *j, size_t cantidad){
    j->puntaje += cantidad;
}

// Terminada

void juego_aumentar_velocidad_actual(juego_t *j){
    j->velocidad_actual *= 1.01; // O algo asi
}

// Terminada

void juego_setear_minutos(juego_t *j, uint32_t minutos){
    j->minutos = minutos;
}

// Terminada

void juego_setear_segundos(juego_t *j, uint32_t segundos){
    j->segundos = segundos;
}

// Terminada

void juego_setear_milisegundos(juego_t *j, uint32_t milisegundos){
    j->milisegundos = milisegundos;
}


// Falta definir correctamente los offsets

imagen_t *juego_generar_frame(juego_t *j, sprites_t *ss){

    imagen_pegar(j->frame, j->fondo, 0, 0);

    if(! imagen_pegar_no_negros(j->frame, tablero_imagen(j->tablero), 0, 16)) // No estoy seguro de los valores de fila y columna (offset)
        return NULL;

    // De estos valores tambien puede estar mal la cosa, la pieza actual tiene su posicion relativa al tableros (vive en él)
    
    if(! imagen_pegar_no_negros(j->frame, pieza_get_imagen(j->pieza_actual), pieza_get_fila(j->pieza_actual) + 0, pieza_get_columna(j->pieza_actual) + 16))
        return NULL;

    // La pieza siguiente tiene posicion fija y relativa a la imagen de fondo

    pieza_t *p_sig_chica = pieza_a_version_chica(j->pieza_siguiente, ss);
    
    if(! imagen_pegar_no_negros(j->frame, pieza_get_imagen(p_sig_chica), pieza_get_fila(p_sig_chica), pieza_get_columna(p_sig_chica)))
        return NULL;

    pieza_destruir(p_sig_chica);

    j->tubo = pieza_crear_tubo(j->pieza_siguiente, ss);

    if(! imagen_pegar(j->frame, pieza_get_imagen(j->tubo), pieza_get_fila(j->tubo), pieza_get_columna(j->tubo))) 
        return NULL;

    // Accedo al puntaje, creo las imagenes representativas del mismo, pego las imagenes en el fondo, destruyo las imagenes creadas

    int c_digitos_puntaje = 0;

    imagen_t **num_puntaje = digito_a_imagen(j->puntaje, &c_digitos_puntaje, ss);

    size_t offset_fil_p = 8 * 14 + 1; // Falta definir lugar correcto
    size_t offset_col_p = 8 * 13 + 1;

    for(size_t i = 0; i < c_digitos_puntaje; i++){
        if(! imagen_pegar(j->frame, num_puntaje[i], offset_fil_p, offset_col_p + i * 8))
            return NULL;
    }

    // Libero las imagenes creadas

    for(size_t i = 0; i < c_digitos_puntaje; i++){
        imagen_destruir(num_puntaje[i]);
    }

    free(num_puntaje);


    // Accedo al nro de clears, creo las imagenes representativas del mismo, pego las imagenes en el fondo, destruyo las imagenes creadas

    int c_digitos_clears = 0;

    imagen_t **num_clears = digito_a_imagen(j->clears, &c_digitos_clears, ss);

    size_t offset_fil_c = 8 * 10 + 1; // Falta definir lugar correcto
    size_t offset_col_c = 8 * 13 + 1;

    for(size_t i = 0; i < c_digitos_clears; i++){
        if(! imagen_pegar_no_negros(j->frame, num_clears[i], offset_fil_c, offset_col_c + i * 8))
            return NULL;
    }

    // Libero las imagenes creadas

    for(size_t i = 0; i < c_digitos_clears; i++){
        imagen_destruir(num_clears[i]);
    }

    free(num_clears);


    // Pego los minutos

    int c_digitos_minutos = 0;

    int c_digitos_totales;
    
    size_t offset_fil_m = 8 * 8 + 1; // Falta definir lugar correcto
    size_t offset_col_m = 8 * 13 + 1;

    if(j->minutos < 10){

        int c_digitos_dec_m = 0;

        c_digitos_totales = 2;

        imagen_t **num_minutos = digito_a_imagen(j->minutos, &c_digitos_minutos, ss);

        uint32_t dec_m = j->minutos / 10;

        imagen_t **dec_minutos = digito_a_imagen(dec_m, &c_digitos_dec_m, ss);
    

        if(! imagen_pegar_no_negros(j->frame, dec_minutos[0], offset_fil_m, offset_col_m))
            return NULL;

        if(! imagen_pegar_no_negros(j->frame, num_minutos[0], offset_fil_m, offset_col_m + 5))
            return NULL;

        // Libero las imagenes creadas (solo una de cada)

        imagen_destruir(dec_minutos[0]);

        free(dec_minutos);

        imagen_destruir(num_minutos[0]);

        free(num_minutos);
    }
    else{

        imagen_t **num_minutos = digito_a_imagen(j->minutos, &c_digitos_minutos, ss);

        c_digitos_totales = c_digitos_minutos;

        for(size_t i = 0; i < c_digitos_minutos; i++){
            if(! imagen_pegar_no_negros(j->frame, num_minutos[i], offset_fil_m, offset_col_m + i * 5))
            return NULL;
        }

        for(size_t i = 0; i < c_digitos_minutos; i++){
            imagen_destruir(num_minutos[i]);
        }
        free(num_minutos);


    }

    // Pego los :

    pieza_t *pieza_dos_puntos = pieza_crear_dos_puntos(ss); // Posicion hipotetica

    if(! imagen_pegar_no_negros(j->frame, pieza_get_imagen(pieza_dos_puntos), pieza_get_fila(pieza_dos_puntos), pieza_get_columna(pieza_dos_puntos))){
        pieza_destruir(pieza_dos_puntos);
        return NULL;
    }

    // Pego los segundos

    int c_digitos_segundos = 0;

    if(j->segundos < 10){

        int c_digitos_dec_s = 0;

        imagen_t **num_segundos = digito_a_imagen(j->segundos, &c_digitos_segundos, ss);

        uint32_t dec_s = j->segundos / 10;

        imagen_t **dec_segundos = digito_a_imagen(dec_s, &c_digitos_dec_s, ss);
    

        if(! imagen_pegar_no_negros(j->frame, dec_segundos[0], offset_fil_m, offset_col_m + 16))
            return NULL;

        if(! imagen_pegar_no_negros(j->frame, num_segundos[0], offset_fil_m, offset_col_m + 21))
            return NULL;

        // Libero las imagenes creadas (solo una de cada)

        imagen_destruir(dec_segundos[0]);

        free(dec_segundos);

        imagen_destruir(num_segundos[0]);

        free(num_segundos);
    }
    else{

        imagen_t **num_segundos = digito_a_imagen(j->segundos, &c_digitos_segundos, ss);

        for(size_t i = 0; i < c_digitos_segundos; i++){
            if(! imagen_pegar_no_negros(j->frame, num_segundos[i], offset_fil_m, offset_col_m + i * 5 + 16))
            return NULL;
        }

        for(size_t i = 0; i < c_digitos_segundos; i++){
            imagen_destruir(num_segundos[i]);
        }
        free(num_segundos);


    }

    // Pego los :

    if(! imagen_pegar_no_negros(j->frame, pieza_get_imagen(pieza_dos_puntos), pieza_get_fila(pieza_dos_puntos), pieza_get_columna(pieza_dos_puntos) + 10)){
        pieza_destruir(pieza_dos_puntos);
        return NULL;
    }

    pieza_destruir(pieza_dos_puntos);

    // Pegos los milisegundos

    int c_digitos_ms = 0;

    if(j->milisegundos < 10){

        int c_digitos_cen_ms = 0;

        int c_digitos_dec_ms = 0;
        
        uint32_t dec_ms = j->milisegundos / 10;

        uint32_t cen_ms = j->milisegundos / 100;

        imagen_t **num_ms = digito_a_imagen(j->milisegundos, &c_digitos_ms, ss);
        
        imagen_t **num_dec_ms = digito_a_imagen(dec_ms, &c_digitos_dec_ms, ss);

        imagen_t **num_cen_ms = digito_a_imagen(cen_ms, &c_digitos_cen_ms, ss);

        if(! imagen_pegar_no_negros(j->frame, num_ms[0], offset_fil_m, offset_col_m +  5 * c_digitos_totales + 10))
            return NULL;

        if(! imagen_pegar_no_negros(j->frame, num_dec_ms[0], offset_fil_m, offset_col_m + 5 * c_digitos_totales + 15))
            return NULL;

        if(! imagen_pegar_no_negros(j->frame, num_cen_ms[0], offset_fil_m, offset_col_m + 5 * c_digitos_totales + 20))
            return NULL;

        // Libero las imagenes creadas (solo una de cada)

        imagen_destruir(num_ms[0]);

        free(num_ms);

        imagen_destruir(num_dec_ms[0]);

        free(num_dec_ms);

        imagen_destruir(num_cen_ms[0]);

        free(num_cen_ms);
    }
    else if(j->milisegundos < 100 && j->milisegundos > 10){

        int c_digitos_cen_ms = 0;

        uint32_t cen_ms = j->milisegundos / 100;

        imagen_t **num_ms = digito_a_imagen(j->milisegundos, &c_digitos_ms, ss);

        imagen_t **num_cen_ms = digito_a_imagen(cen_ms, &c_digitos_cen_ms, ss);

        if(! imagen_pegar_no_negros(j->frame, num_ms[0], offset_fil_m, offset_col_m + 5 * c_digitos_totales + 10))
            return NULL;

        if(! imagen_pegar_no_negros(j->frame, num_ms[1], offset_fil_m, offset_col_m + 5 * c_digitos_totales + 15))
            return NULL;

        if(! imagen_pegar_no_negros(j->frame, num_cen_ms[0], offset_fil_m, offset_col_m + 5 * c_digitos_totales + 20))
            return NULL;

        // Libero las imagenes creadas (solo una de cada)

        imagen_destruir(num_ms[0]);

        imagen_destruir(num_ms[1]);
        
        free(num_ms);

        imagen_destruir(num_cen_ms[0]);

        free(num_cen_ms);
    }
    else{

        imagen_t **num_segundos = digito_a_imagen(j->segundos, &c_digitos_segundos, ss);

        c_digitos_totales = c_digitos_segundos;

        for(size_t i = 0; i < c_digitos_segundos; i++){
            if(! imagen_pegar_no_negros(j->frame, num_segundos[i], offset_fil_m, offset_col_m + (i + c_digitos_totales + 2) * 5))
            return NULL;
        }

        for(size_t i = 0; i < c_digitos_segundos; i++){
            imagen_destruir(num_segundos[i]);
        }
        free(num_segundos);


    }

    return j->frame;

}

