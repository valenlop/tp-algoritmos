
#include "color.h"

#include <math.h>

#define SHIFT_F 5 
#define SHIFT_E 6
#define VIVEN6B 0x3f 
#define VIVEN5B 0x1f 

// (1)

color_t color_desde_def(uint8_t d, uint8_t e, uint8_t f){
    color_t n = 0;

    n = (n | d) << SHIFT_E; 
    n = (n | e) << SHIFT_F; 
    n = (n | f);            

    return n;
}

// (2) 

uint8_t color_d(color_t c){

    return c >> (SHIFT_F + SHIFT_E);
}

// (3)

uint8_t color_e(color_t c){

    return (c >> (SHIFT_F)) & VIVEN6B;
}

// (4)

uint8_t color_f(color_t c){

    return c & VIVEN5B;
}

// (5) 

color_t color_desde_rgb(uint8_t r, uint8_t g, uint8_t b){

    uint8_t max; 
    
    if(r >= g){
        if(r >= b){
            max = r;
        }
        else {
            max = b;
        }
    }
    else {
        if(g >= b){
            max = g;
        }
        else {
            max = b;
        }
    }
   
    uint8_t min; 

    if(r <= g){
        if(r <= b){
            min = r;
        }
        else {
            min = b;
        }
    }
    else {
        if(g <= b){
            min = g;
        }
        else {
            min = b;
        }
    }

    uint8_t f = (max + min) >> 4; 

    uint8_t d;
    uint8_t e;

    // Zona de D
    
    if(max == min){
        d = 0;  // Vale esto
    }
    
    else if(f < 16){
        d = (((max - min) << 5) / (max + min)) - 1; 
    }

    else {
        d = ((max - min) << 5) / (512 - max - min);
    }


    // Zona de E

    if(max == min){
        e = 0;
    }
    
    else if(max == r){
        e = ((11 * (g - b)) / (max - min)) + 64; 
    }

    else if(max == g){
        e = ((11 * (b - r)) / (max - min)) + 21; 
    }

    else {
        e = ((11 * (r - g)) / (max - min)) + 43; 
    }

    

    e &= VIVEN6B; 
    

    return color_desde_def(d, e ,f);
}

// (6) 

void color_a_rgb(color_t c, uint8_t *r, uint8_t *g, uint8_t *b){

    uint8_t d = color_d(c);   
    uint8_t e = color_e(c);   
    uint8_t f = color_f(c);  

    
    int cc;
    
    cc = (8 - abs((f >> 1) - 8)) * d;

    int x = cc * (1 - fabs(fmodf(e / 11.0, 2) - 1));

    int m = (f << 3) - (cc >> 1);
    int h = e / 11; 

    int rr;
    int gg;
    int bb;

    if(h == 0){
        rr = cc;
        gg = x;
        bb = 0;
    }
    else if(h == 1){
        rr = x;
        gg = cc;
        bb = 0;
    }
    else if(h == 2){
        rr = 0;
        gg = cc;
        bb = x;
    }
    else if(h == 3){
        rr = 0;
        gg = x;
        bb = cc;
    }
    else if(h == 4){
        rr = x;
        gg = 0;
        bb = cc;
    }
    else {
        rr = cc;
        gg = 0;
        bb = x;
    }


    if(rr + m > 255){
        *r = 255;
    }
    else {
        *r = rr + m;
    }

    if(gg + m > 255){
        *g = 255;
    }
    else{
        *g = gg + m;
    }

    if(bb + m > 255){
        *b = 255;
    }
    else{
        *b = bb + m;
    }

}