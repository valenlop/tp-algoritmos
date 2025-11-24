
// TDA: Trabajar con colores ((Terminado))

#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef uint16_t color_t;

color_t color_desde_def(uint8_t d, uint8_t e, uint8_t f);

uint8_t color_d(color_t c);

uint8_t color_e(color_t c);

uint8_t color_f(color_t c);

color_t color_desde_rgb(uint8_t r, uint8_t g, uint8_t b);

void color_a_rgb(color_t c, uint8_t *r, uint8_t *g, uint8_t *b);

#endif