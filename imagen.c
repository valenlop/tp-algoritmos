#include "imagen.h"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>



struct imagen {
    // pixeles es un arreglo de arreglos de color_t de una imagen
    // ancho y alto son el ancho y el alto de una imagen
    // Si ancho = 0 o alto = 0 <=> pixeles == NULL

    color_t **pixeles;
    size_t ancho, alto;
};



// (8) 

void imagen_destruir(imagen_t *i){


    for(size_t j = 0; j < i->alto; j++){   
        free(i->pixeles[j]);
    }

    free(i->pixeles);

    free(i);
}

// (7)

imagen_t *imagen_crear(size_t ancho, size_t alto){

   imagen_t *im = malloc(sizeof(imagen_t)); 
    if(im == NULL) return NULL;          

    im->ancho = ancho;
    im->alto = 0;

    
    im->pixeles = malloc(alto * sizeof(color_t *));
        if(im->pixeles == NULL){ 
            free(im);
            return NULL;
        }

    for(size_t i = 0; i < alto; i++){
        im->pixeles[i] = malloc(ancho * sizeof(color_t));
            
            im->alto = i + 1; 

            if(im->pixeles[i] == NULL){
                imagen_destruir(im);
                return NULL;
            }   
    }

    
   return im;
}

// (9)

imagen_t *imagen_clonar(const imagen_t *i){

    imagen_t *copia = imagen_crear(i->ancho, i->alto);
        if(copia == NULL)
            return NULL;
    
    

    for(size_t j = 0; j < i->alto; j++){
        for(size_t z = 0; z < i->ancho; z++){
            copia->pixeles[j][z] = i->pixeles[j][z];
        }
    }

    return copia;
}

// (10)

void imagen_escribir_ppm(const imagen_t *i){
    
    printf("P3\n");

    printf("%zu %zu\n", i->ancho, i->alto);

    printf("255\n");


    for(size_t f = 0; f < i->alto; f++){
        for(size_t c = 0; c < i->ancho; c++){
            
            color_t pix = i->pixeles[f][c];
            uint8_t r;
            uint8_t g;
            uint8_t b;
            
            
            color_a_rgb(pix, &r, &g, &b);

            printf("%u  \n", r);
            printf("%u  \n", g);
            printf("%u  \n", b);
        }
    }
}


// (11)

imagen_t *imagen_leer_ppm(){


    char aux[100];
    char aux2[100];

    while(fgets(aux, 100, stdin) != NULL){
        
        if(aux[0] == '#'){
            continue;
        }
        else if(!strcmp(aux, "P3\n")){
            break;
        }
        else {
            return NULL;
        }
    }


    while(fgets(aux, 100, stdin) != NULL){
        
        if(aux[0] == '#'){
            continue;
        }
        else if(atoi(aux) == 0){
            return NULL;
        }
        
        size_t i;

        for(i = 0; aux[i] != ' '; i++){
        }

        for(size_t j = 0; (aux2[j] = aux[j + i]); j++);

        if(atoi(aux2) == 0){
            return NULL;
        }
    
        else {
            break;
        }   
    } 
            size_t ancho = atoi(aux);
            size_t alto = atoi(aux2);
        
        


    imagen_t *imagen = imagen_crear(ancho, alto); 

    while(fgets(aux, 100, stdin) != NULL){
        if(aux[0] == '#'){
            continue;
        }
        
        else if(atoi(aux) != 255){
            return NULL;
        }
        
        else {
            break;
        }
    }


    for(size_t f = 0; f < alto; f++){
        for(size_t c = 0; c < ancho; c++){
            
            int r;
            int g;
            int b;


            while(fgets(aux, 100, stdin) != NULL){
                    if(aux[0] == '#'){
                        continue;
                    }
                    else {
                    r = atoi(aux);
                    
                    fgets(aux, 100, stdin);
                    g = atoi(aux);
                    
                    fgets(aux, 100, stdin);
                    b = atoi(aux);

                    break;
                    }

            }
            imagen->pixeles[f][c] = color_desde_rgb(r, g, b);
        }
    }

    return imagen;
}


// Implementacion de lo que falta


// Esta funcion concede fila y columna = 0 (sirve para iteraciones)

bool imagen_setear_pixel(imagen_t *i, size_t fila, size_t columna, color_t color){
    if(i == NULL){
        return false;
    }
    
    if(fila >= i->alto || columna >= i->ancho){
        return false;
    }

    i->pixeles[fila][columna] = color;
    return true;
}

// Esta concede fila y columna = 0 (la uso en iteraciones)

color_t imagen_obtener_pixel(imagen_t *i, size_t fila, size_t columna){
    // Aca voy a asumir que nunca i == NULL y que fila y columna estan en rango

    return i->pixeles[fila][columna];
}


// Toma una imagen y le pega en otra con un offset (util para el tablero), como puede fallar devuelve un bool

bool imagen_pegar_no_negros(imagen_t *destino, imagen_t *origen, size_t sf, size_t sc){
    if(origen == NULL){
        return false;
    }

    if(sf + origen->alto > destino->alto || sc + origen->ancho > destino->ancho){
        return false;
    }

    for(size_t f = 0; f < origen->alto; f++){
        for(size_t c = 0; c < origen->ancho; c++){
            if(imagen_obtener_pixel(origen, f, c)){
                destino->pixeles[f + sf][c + sc] = origen->pixeles[f][c];
            }
        }
    }

    return true;
}

// Deberia andar ((NO LO PROBE))

imagen_t *imagen_rotar(imagen_t *i){
    if(i == NULL){
        return NULL;
    }

    imagen_t *imagen_rotada = imagen_crear(i->alto, i->ancho);
    if(imagen_rotada == NULL){
        return NULL;
    }

    for(size_t f = 0; f < i->alto; f++){
        for(size_t c = 0; c < i->ancho; c++){
            imagen_rotada->pixeles[i->ancho - 1 - c][f] = i->pixeles[f][c]; // ((DEBERIA SER ASI))
        }
    }

    return imagen_rotada;

}

// Por ahora no veo si debe ir en este TDA o en otro ((PREGUNTAR))

// Si el color viene fijo no sirve de mucho (o capaz si) (ver que onda con pieza_crear)

void imagen_iniciar_color(imagen_t *i, color_t color){
    
}

size_t imagen_ancho(imagen_t *i){
    return i->ancho;
}

size_t imagen_alto(imagen_t *i){   
    return i->alto;
}


