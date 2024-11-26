# Grupo 10
Ignacio Jesús Soto Miranda
Benjamín Ignacio Díaz Ulloa
Diego Antonio Matus Salas

Para compilar: gcc -o main main.c game.c prints.c -L./src/lib -I./src/include -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

(Programa compila correctamente en version gcc (Rev2, Built by MSYS2 project) 14.2.0 de Windows)

Para abrir, ejecutar en cmd: main <ubicación de mapa>

Se tiene disponible mapa.txt como ejemplo.

El mapa es de 28 x 32, en donde:

    -1 son los bloques
    0 son los espacios vacíos
    1 son las monedas
    2 es la posición inicial de Pacman
    6 es la posición inicial del fantasma celeste
    7 es la posición inicial del fantasma naranja
    8 es la posición inicial del fantasma rojo
    9 es la posición inical del fantasma rosa


Qué implementaciones tiene nuestro programa:
1) Lectura de entrada desde un archivo.
2) Desplazamiento dentro del laberinto de fantasmas y Pac-man
3) Visualización del laberinto
4) Almacenamiento y cambio de estados
5) Mejora en la jugabilidad: Contador de puntos recolectados, contador de vidas de pacman,
portales como el juego original.
6) Mejora en la visualización: Se usó biblioteca SDL2/SDL2_image para poder usar sprites en vez de ascii art,
se usó SDL2_mixer para sonidos para ciertos eventos, y SDL2_ttf para poder usar fuentes para los contadores.
7) De manera extra, también hay un programa para la creación de mapas nuevos a través de imagenes en vez de texto.