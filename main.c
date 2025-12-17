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

    srand((unsigned)time(NULL));

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

    uint32_t tiempo_anterior = SDL_GetTicks();

    static double acumulado_caida_rapida = 0;

    static int acumulador_izq = 0;
    static int acumulador_der = 0;

    while(1) {

        uint32_t tiempo_actual = SDL_GetTicks();
        uint32_t delta_ms = tiempo_actual - tiempo_anterior;
        tiempo_anterior = tiempo_actual;
        
        pieza_t *p = juego_obtener_pieza_actual(juego);
        tablero_t *t = juego_obtener_tablero(juego);

        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;

            // BEGIN código del alumno

            uint32_t ahora = SDL_GetTicks(); // Para calcular el tiempo {}
            uint32_t transcurrido = ahora - tiempo_inicio;

            juego_setear_minutos(juego, transcurrido / 60000);
            juego_setear_segundos(juego, (transcurrido % 60000) / 1000);
            juego_setear_milisegundos(juego, transcurrido % 1000);

            // Velocidad por tiempo

            static uint32_t ultimo_segundo = 0;
            uint32_t segundo_actual = transcurrido / 1000;

            if (segundo_actual > ultimo_segundo) {
                ultimo_segundo = segundo_actual;
                juego_aumentar_velocidad_actual(juego);
            }

            // Tiempo ya seteado

            if (event.type == SDL_KEYDOWN) {
                
                switch(event.key.keysym.sym) {
                    case SDLK_LEFT:
                        acumulador_izq = 4;  // 4 cuadros para mover 8 columnas
                        break;

                    case SDLK_RIGHT:
                        acumulador_der = 4;
                        break;

                    case SDLK_UP:

                        pieza_rotar(p);
                        if (tablero_colision(t, p)) {
                            pieza_rotar(p);
                            pieza_rotar(p);
                            pieza_rotar(p);
                        }
                        break;

                    case SDLK_DOWN:
                        acumulado_caida_rapida = 333;
                        break;
                }
            }
            // END código del alumno

            continue;
        }

        // BEGIN código del alumno

        // Mover izquierda si queda acumulador
        if(acumulador_izq > 0) {
            pieza_mover_columna(p, -2);  // 2 columnas por cuadro -> 4 cuadros = 8 columnas
            if(tablero_colision(t, p))
                pieza_mover_columna(p, +2);
            acumulador_izq--;
        }

        // Mover derecha si queda acumulador
        if(acumulador_der > 0) {
            pieza_mover_columna(p, +2);
            if(tablero_colision(t, p))
                pieza_mover_columna(p, -2);
            acumulador_der--;
        }
        
        /* ---- CAÍDA AUTOMÁTICA DE LA PIEZA ---- */
        static double acumulado = 0;
        acumulado += delta_ms;

        double velocidad = juego_obtener_velocidad_actual(juego);


        if(acumulado_caida_rapida > 0){
            velocidad *= 5;
            acumulado_caida_rapida -= delta_ms;
            
            if(acumulado_caida_rapida < 0){
                acumulado_caida_rapida = 0;
            }
        }

        uint32_t delay = (uint32_t)(1000 / velocidad);

        if (acumulado >= delay) {
            acumulado -= delay;

            pieza_mover_fila(p, 1);

            if (tablero_colision(t, p)) {

                /* ---- PEGAR PIEZA EN TABLERO ---- */
                tablero_pegar_pieza(t, p);

                /* ---- NUEVA PIEZA ---- */
                juego_setear_pieza_actual(juego, juego_obtener_pieza_siguiente(juego));
                pieza_t *p_siguiente = pieza_crear(figuras);
                juego_setar_pieza_siguiente(juego, p_siguiente);

                /* ---- GAME OVER ---- */
                if (tablero_perdio(t, juego_obtener_pieza_actual(juego))) {
                    break;
                }
            }
        }
        

        /* ---- SIMULAR ARENA (3 veces) ---- */
        for (int i = 0; i < 3; i++) {
            tablero_simular_arena(t);

            size_t puntaje = 0;
            while (tablero_linea_formada(t, &puntaje)) {
                juego_aumentar_nro_clears(juego);
                juego_aumentar_puntaje(juego, puntaje);
            }
        }

        /* ---- GENERAR FRAME ---- */
        imagen_t *frame = juego_generar_frame(juego, figuras);

        /* ---- PASAR imagen_t (RGB565) A canvas (RGBA8888) ---- */
        memset(canvas, 0, sizeof(canvas));

        for (size_t f = 0; f < imagen_alto(frame); f++) {
            for (size_t c = 0; c < imagen_ancho(frame); c++) {
                color_t cc = imagen_obtener_pixel(frame, f, c);
                uint8_t r, g, b;
                color_a_rgb(cc, &r, &g, &b);

                canvas[f * VENTANA_ANCHO + c] =
                    (r << 24) | (g << 16) | (b << 8) | 0xFF;
            }
        }
        
        // END código del alumno

        SDL_UpdateTexture(texture, NULL, canvas, VENTANA_ANCHO * sizeof(uint32_t));
        // SDL_RenderCopy(renderer, texture, NULL, NULL); // Toque aca para que se vea mas grande
        SDL_Rect dest = {0, 0, VENTANA_ANCHO*4, VENTANA_ALTO*4}; // Despues borrar esto
        SDL_RenderCopy(renderer, texture, NULL, &dest); // despues borrar esto

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
