// Ideas

// Tengo "4" TDAs, no se si voy a usar los 4 pero al menos estan modularizados como para su uso

// Tengo que leer el tp y partirlo en varias partes



// Resumen:

// SDL2: Me dan el esqueleto, tengo que leer el teclado, dibujar puntos y cambiar el color (se indica donde en test_sdl2.c)


// COLORES DEF: Formato SHL (saturacion, gama, luminosidad). E = 0 (rojo), 11 (amarillo), 22 (verde), 44 (azul). ((ver apunte: numeros pseudoaleatorios))
// el 0x00 es el negro y 0xff es el blanco


// JUEGO: Cae una pieza, la misma se puede rotar, mover a la izquierda, derecha o apurarla hacia abajo. 
//        Si la pieza toca algun pixel de color distinto de negro, pasar a ser parte de la arena (cada grano debe ir actualizando su posicion)
//        Elimino granos formando caminos de izquierda a derecha (recursividad)

// PIEZAS: Letras: I, L, J, S, Z, T y O. Cada caja ocupa 8x8 pixeles, ej: la I ocuparia 8x32 pixeles.
//         Para el tablero usamos los sprites "i", para el anticipo de la proxima pieza "ic" version chica (cajas de 4x4)
//         Mover las piezas se hace de a 1 a la vez pero 8 unidades
//         Para la flecha de arriba (se rota 90° en sentido antihorario)
//         Las piezas no se escapan del tablero => no siempre se pueden mover, si hay giro tal vez deba ser empujada para quedar en el tablero
//         Las piezas caen del centro y desde el tope


// MAS SPRITES: Hay sprites con caracteres. Sirve para imprimir texto en la pantalla


// TABLERO: Mide 80x144 pixeles, todos los pixeles en negro. Las piezas que caen no son parte del tablero hasta chocar con algo (otro color o fin)
//          Pegar la pieza es pegar una imagen en otra imagen: 
//          Para actualizar tablero: Avanzar en uno la simulacion de arena y verificar que no se haya formado una linea.


// SIMULACION DE ARENA: Automata finito. Los granos no se crean ni se destruyen, se desplazan. Partimos de un tablero anterior y generamos uno nuevo.
/*
    Reglas: - Nuevo tablero todo en negro
            - La fila inferior al tablero se copia tal cual
            - Para cada pixel no negro: (3) casos:
                    - Si generar random(4) es distinto de cero, el pixel se copia a la misma posición en el tablero nuevo.
                    - Si no y la posición debajo de ese pixel se encuentra vacía en el tablero viejo entonces el pixel se copia una fila más abajo.
                    -  Si no y la alguna de las dos posiciones debajo a izquierda y derecha es decir (f + 1, c +- 1) están libres 
                        se eligirá aleatoriamente una de las dos (o sea, si sólo una está libre será esa pero si están libres las dos se debe optar al azar).
                        - píxeles de la primera y última columna no tienen posición a izquierda y derecha respectivamente.
*/


// DETECCION DE LINEA: Para bloques con el mismo E (mismo color): 
/*
Para todos los pixeles de la primera columna: 
    - Si el pixel no es negro y no fue visitado previamente => se visita con su color correspondiente

- Si el pixel ya fue visitado => se devuelve negativo
 - Si el pixel no es del color buscado, entonces el algoritmo devuelve negativo.
 - Si estamos en la última columna, entonces se devuelve positivo.
 - Si no, se marca como visitada la fila y columna actual y se visitan los pixeles que estén a derecha, arriba, abajo e izquierda del actual. 
    Si alguna de esas visitas devuelve positivo se devuelve positivo. 
    Notar el orden de las operaciones, respetarlo, es más probable que lleguemos a la última columna si avanzamos primero hacia la derecha.

Para llevar la cuenta de posiciones visitadas se puede generar una nueva imagen del mismo tamaño del tablero o una matriz e 
ir marcando los pixeles ya conocidos.

Notar que la operación de visitar en los 4 sentidos puede escaparse del tablero, en ese caso la respuesta es siempre negativa.

Notar que este algoritmo dado, con estas reglas dadas, no recorre todos los pixeles del bloque,
sólo se garantiza que haya un camino desde la primera a la última columna.

Una vez confirmada la linea tengo que diseñar algo para eliminar el bloque completo
*/


// PUNTAJE: Sera la cantidad de pixeles eliminados hasta el momento


// EVOLUCION DEL JUEGO: ((No entendi del todo)) ((REVISAR))