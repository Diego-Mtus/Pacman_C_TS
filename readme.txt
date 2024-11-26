Debido a que las librerias de SDL tienen versión de 32 y 64 bits, se tuvo que crear 2 versiones del juego, usando cada una de las librerias (esto puede arreglar ciertos problemas a la hora de compilar si la versión de gcc es de 32 o 64 bits)

** El programa fue hecho usando gcc y librerias de 64 bits **

# Grupo 10
Ignacio Jesús Soto Miranda
Benjamín Ignacio Díaz Ulloa
Diego Antonio Matus Salas

Para compilar: 
gcc -o main main.c game.c prints.c -L./src/lib -I./src/include -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer