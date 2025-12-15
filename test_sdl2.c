#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "config.h"

#include "juego.h"

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Sandtrix");

    SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, VENTANA_ANCHO, VENTANA_ALTO);
    uint32_t canvas[VENTANA_ALTO * VENTANA_ANCHO];

    int dormir = 0;

    // BEGIN código del alumno
    FILE *fondo = fopen("fondo.ppm", "rt");
    if(fondo == NULL){
        fprintf(stderr, "La cago el fondo");
        return 1;
    }

    imagen_t *im_fondo = imagen_leer_ppm(fondo);
    if(im_fondo == NULL){
        fclose(fondo);
        fprintf(stderr, "im_fondo la cago");
        return 1;
    }

    fclose(fondo);

    sprites_t *figuras = sprites_crear("sprites.bin");

    

    juego_t *juego = juego_crear(im_fondo, figuras);
    if(juego == NULL){
        sprites_destruir(figuras);
        fprintf(stderr, "Fallo crear juego");
        return 1;
    }

    // END código del alumno

    unsigned int ticks = SDL_GetTicks();

    uint32_t tiempo_inicio = SDL_GetTicks(); // Para calcular el tiempo

    while(1) {
        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;

            // BEGIN código del alumno

            uint32_t ahora = SDL_GetTicks(); // Para calcular el tiempo {}
            uint32_t transcurrido = ahora - tiempo_inicio;

            uint32_t minutos = transcurrido / 60000;
            uint32_t segundos = (transcurrido % 60000) / 1000;
            uint32_t milisegundos = transcurrido % 1000; // Para calcular el tiempo {}

            juego_setear_minutos(juego, minutos);
            juego_setear_segundos(juego, segundos);
            juego_setear_milisegundos(juego, milisegundos);

            // Tiempo ya seteado

            if (event.type == SDL_KEYDOWN) {
                // Se apretó una tecla
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        fi -= 10;
                        break;
                    case SDLK_DOWN:
                        fi += 10;
                        break;
                    case SDLK_RIGHT:
                        ci += 10;
                        break;
                    case SDLK_LEFT:
                        ci -= 10;
                        break;
                }
            }
            // END código del alumno

            continue;
        }

        // BEGIN código del alumno
        memset(canvas, 0, VENTANA_ALTO * VENTANA_ANCHO * sizeof(uint32_t));
        for(size_t f = 0; f < imagen_alto(im); f++)
            for(size_t c = 0; c < imagen_ancho(im); c++) {
                color_t cc = imagen_obtener(im, f, c);
                uint8_t r, g, b;
                color_a_rgb(cc, &r, &g, &b);
                if((fi + f) * 4 >= VENTANA_ALTO || (ci + c) * 4 >= VENTANA_ANCHO) continue;
                for(size_t i = 0; i < 4; i++)
                    for(size_t j = 0; j < 4; j++)
                        canvas[((fi + f) * 4 + i) * VENTANA_ANCHO + (ci + c) * 4 + j] = r << 24 | g << 16 | b << 8;
            }
        // END código del alumno

        SDL_UpdateTexture(texture, NULL, canvas, VENTANA_ANCHO * sizeof(uint32_t));
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);
        ticks = SDL_GetTicks() - ticks;
        if(dormir) {
            SDL_Delay(dormir);
            dormir = 0;
        }
        else if(ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        else
            printf("Perdiendo cuadros\n");
        ticks = SDL_GetTicks();
    }

    // BEGIN código del alumno

    juego_destruir(juego); // Libero toda la memoria asociada al juego y listo

    // END código del alumno

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
