
// TDA: Lo hice TDA tal vez no es necesario

#ifndef MATRIZ_H
#define MATRIZ_H

#include <stddef.h>
#include <stdbool.h>

void inicializar_matriz(size_t filas, size_t columnas, char matriz[filas][columnas]);

void copiar_matriz(size_t filas, size_t columnas, char destino[filas][columnas], char origen[filas][columnas]);

void imprimir_ppm(size_t filas, size_t columnas, char matriz[filas][columnas]);

bool pegar_submatriz(size_t mfs, size_t mcs, char matriz[mfs][mcs], size_t sfs, size_t scs, char submatriz[sfs][scs], size_t fi, size_t ci);

int sumar_entorno(size_t filas, size_t columnas, char matriz[filas][columnas], size_t fc, size_t cc);

void evolucionar_celulas(size_t filas, size_t columnas, char final[filas][columnas], char inicial[filas][columnas]);

#endif