#include "imagen.h"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>



struct imagen {
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
