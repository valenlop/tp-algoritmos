#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

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

    srand((unsigned)time(NULL)); // Inicio la semilla para random

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

    int mover_izq = 0;

    int mover_der = 0;

    int caer_rapido = 0;

    

    double delta_tiempo = 1000.0 / JUEGO_FPS; // Espacio de tiempo entre frames

    double timer_velocidad = 1000; // Lo uso para aumentar velocidad

    double posi_real_pieza = 0;

    // END código del alumno

    unsigned int ticks = SDL_GetTicks(); // Medir tiempo de inicio

    while(1) {

        tablero_t *t = juego_obtener_tablero(juego);
        pieza_t *p = juego_obtener_pieza_actual(juego);

        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;

            // BEGIN código del alumno

            if (event.type == SDL_KEYDOWN) {
                
                switch(event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Ver que hacer con tecla izquierda (desplazar 8 pixeles en 4 cuadros)
                        mover_izq = 4;
                        break;

                    case SDLK_RIGHT:
                        // Ver que hacer con tecla derecha (desplazar 8 pixeles en 4 cuadros)
                        mover_der = 4;
                        break;

                    case SDLK_UP:
                        // Ver que hacer con tecla arriba (rotar pieza)
                        pieza_rotar(p);
                        if(tablero_colision(t, p)){
                            pieza_rotar(p);
                            pieza_rotar(p);
                            pieza_rotar(p);
                        }
                        break;

                    case SDLK_DOWN:
                        // Ver que hacer con tecla abajo (aumentar velocidad * 5)
                        caer_rapido = 333;
                        break;
                }
            }

            // END código del alumno

            continue;
        }

        // BEGIN código del alumno

        
        
        
        
        double velocidad = juego_obtener_velocidad_actual(juego);

        juego_setear_minutos(juego, ticks / 60000); // Dato minutos

        juego_setear_segundos(juego, (ticks / 1000) % 60); // Dato segundos

        juego_setear_milisegundos(juego, ticks % 1000); // Dato milisegundos

        // Mover izquierda 
        if(mover_izq > 0) {
            pieza_mover_columna(p, -1);  // Muevo de a uno
            if(tablero_colision(t, p))
                pieza_mover_columna(p, +1);

            pieza_mover_columna(p, -1);  // Muevo de a uno
            if(tablero_colision(t, p))
                pieza_mover_columna(p, +1);
            mover_izq--;
        }

        // Mover derecha 
        if(mover_der > 0) {
            pieza_mover_columna(p, +1);
            if(tablero_colision(t, p))
                pieza_mover_columna(p, -1);

            pieza_mover_columna(p, +1);
            if(tablero_colision(t, p))
                pieza_mover_columna(p, -1);
            mover_der--;
        }
        
        // Proceso de caida de la pieza

        if(timer_velocidad > 0){
            timer_velocidad -= delta_tiempo;
            if(timer_velocidad <= 0){
                juego_aumentar_velocidad_actual(juego);
                timer_velocidad += 1000;
            }
        }

        if(caer_rapido > 0){
            velocidad *= 5;
            caer_rapido -= delta_tiempo;

            if(caer_rapido < 0){
                caer_rapido = 0;
            }
        }

        double paso = velocidad * delta_tiempo / 1000; // El step de la pieza

        posi_real_pieza += paso; // Posicion real de la pieza

        while(posi_real_pieza - 1 > pieza_get_fila(p)){

                pieza_mover_fila(p, 1);

                if (tablero_colision(t, p)) {
                
                    // Cerrar juego

                    if (tablero_perdio(t, juego_obtener_pieza_actual(juego))) {
                        break;
                    }

                    // Pegar pieza

                    tablero_pegar_pieza(t, p);

                    // Generar pieza nueva y pasar a actual

                    juego_setear_pieza_actual(juego, juego_obtener_pieza_siguiente(juego));
                    pieza_t *p_siguiente = pieza_crear(figuras);
                    juego_setar_pieza_siguiente(juego, p_siguiente);

                    posi_real_pieza = (double)pieza_get_fila(juego_obtener_pieza_actual(juego));
                    break;
                }
        }

        if(tablero_perdio(t, juego_obtener_pieza_actual(juego))){
            break;
        }
        

        // Simulacion de arena + lineas

        for (int i = 0; i < 3; i++) {
            tablero_simular_arena(t);

            size_t puntaje = 0;
            while (tablero_linea_formada(t, &puntaje)) {
                juego_aumentar_nro_clears(juego);
                juego_aumentar_puntaje(juego, puntaje);
            }
        }

        // Genero frame

        imagen_t *frame = juego_generar_frame(juego, figuras);

        // Paso imagen a formato de SDL

        for (size_t f = 0; f < imagen_alto(frame); f++) {
            for (size_t c = 0; c < imagen_ancho(frame); c++) {

                color_t cc = imagen_obtener_pixel(frame, f, c);

                uint8_t r, g, b;
                color_a_rgb(cc, &r, &g, &b); 

                // Se ve que es asi el formato

                canvas[f * VENTANA_ANCHO + c] = (r << 24) | (g << 16) | (b << 8) | 0xFF;
                }
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

    sprites_destruir(figuras);

    juego_destruir(juego); // Libero toda la memoria asociada al juego y listo

    // END código del alumno

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
