// Desglose de idea ((mas detalle))

/*
Cosas que tengo:

color_h: Formato SHL y RGB util para definir los tetraminos de colores y dibujar con SDL2 (ya hecho)

imagen_h: Manejar el tablero como imagen, pegar piezas, leer/escribir PPM para testear. (falta completar)
    Tengo que adaptar matriz.h (funciones)
    falta:  - imagen_setear_pixel
            - imagen_establecer_pixel
            - imagen_pegar_no_negros
            - imagen_rotar

sprite_h/sprites_h: Leer tetraminos y textos (numeros, letras) + proximas piezas y puntaje (ya hecho)


TDAs a implementar (tal vez):
    
    - pieza.h: Una pieza actual (la pieza que cae). La pieza y el tablero son dos cosas distintas.


    - tablero.h: Laburo del tablero. Va la imagen de fondo y los tetraminos que chocaron.


    - juego.h: Controla toda la logica de juego. Movimientos de las piezas, tiempo y demas dibujos en pantalla.



*/