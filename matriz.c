
#include "matriz.h"

#include <stdio.h>


// (1)

void inicializar_matriz(size_t filas, size_t columnas, char matriz[filas][columnas]){
    for(size_t f = 0; f < filas; f++)
        for(size_t c = 0; c < columnas; c++)
            matriz[f][c] = 0;
}

// (2)

void copiar_matriz(size_t filas, size_t columnas, char destino[filas][columnas], char origen[filas][columnas]){
    for(size_t f = 0; f < filas; f++)
        for(size_t c = 0; c < columnas; c++)
            destino[f][c] = origen[f][c];
}

//(3)

void imprimir_ppm(size_t filas, size_t columnas, char matriz[filas][columnas]){
    printf("P1\n");
    printf("%zd %zd\n", columnas, filas);
    
    for(size_t f = 0; f < filas; f++)
        for(size_t c = 0; c < columnas; c++){
            printf("%d\n" ,matriz[f][c]);
        }
}

// (4)

bool pegar_submatriz(size_t mfs, size_t mcs, char matriz[mfs][mcs], size_t sfs, size_t scs, char submatriz[sfs][scs], size_t fi, size_t ci){
    if(mfs < sfs + fi || mcs < scs + ci){
        return false;
    }
    for(size_t f = 0; f < sfs; f++)
        for(size_t c = 0; c < scs; c++){
            matriz[f + fi][c + ci] = submatriz[f][c];
        }
    return true;
}

// (5)

int sumar_entorno(size_t filas, size_t columnas, char matriz[filas][columnas], size_t fc, size_t cc){
    if(fc <= 0 || fc >= filas - 1 || cc <= 0 || cc >= columnas -1){
        return 0;
    }
    
    int suma = 0;

    for(size_t f = fc - 1; f < fc + 2; f++){
        for (size_t c = cc - 1; c < cc + 2; c++){
            suma += matriz[f][c];
        }
    }
    suma -= matriz[fc][cc];
    return suma;
}

// (6)

void evolucionar_celulas(size_t filas, size_t columnas, char final[filas][columnas], char inicial[filas][columnas]){
    
    for(size_t f = 1; f < filas - 1; f++){
        for(size_t c = 1; c < columnas - 1; c++){
            if(inicial[f][c] == 0){
                int n = sumar_entorno(filas, columnas, inicial, f, c);
                if(n == 3){
                    final[f][c] = 1;
                } 
                else{
                    final[f][c] = inicial[f][c];
                }
            }
           
            else if(inicial[f][c] == 1){
                int m = sumar_entorno(filas, columnas , inicial, f, c);
                if(m == 2 || m == 3){
                    final[f][c] = inicial[f][c];
                } 
                else{
                    final[f][c] = 0;
                }
            }
           
            else {
                final[f][c] = inicial[f][c];
            }
        }

    }
}

