#include <stdio.h>
#include <stdlib.h>

// Para BMP, los 2 primeros bytes son BM (magic number), luego 4 bytes para tamaño del archivo, 4 bytes reservados, 4 bytes para offset de la imagen 
// (En donde empieza la información de la imagen).
// Con lo que para BMP 24 bits de una imagen de 28x32, el tamaño del archivo sería 54 + 28*32*3 = 2862 bytes.
// Información de la imagen empezaría en el byte 54.

// Con esto, podemos leer la imagen de mapa.bmp y crear un archivo mapa.txt con los valores de los pixeles de la imagen.

// !! ES IMPORTANTE QUE EL INPUT SEA UNA IMAGEN BMP-24 DE 28x32 PIXELES !!

// Guía de uso en readmeLeerBPM.txt

int main(int argc, char * argv[]){

    if(argc!=2) {
        printf("Has olvidado ingresar imagen. El formato es: %s <ubicación del mapa> \n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "rb");
    FILE *mapa = fopen("mapaImagen.txt", "w");

    if (file == NULL){
        printf("Error al abrir el archivo mapa.\n");
        return EXIT_FAILURE;
    }

    if (mapa == NULL){
        printf("Error al abrir el archivo mapaImagen.txt\n");
        return EXIT_FAILURE;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    // Verificación de archivo BMP
    if (header[0] != 'B' || header[1] != 'M'){
        printf("El archivo no es un BMP\n");
        return EXIT_FAILURE;
    } else {
        printf("Archivo BMP correcto!\n");
    }

    int ancho = header[18] + header[19]*256;
    int alto = header[22] + header[23]*256;

    printf("Ancho: %d, Alto: %d\n", ancho, alto);
    if (ancho != 28 || alto != 32){
        printf("El tamaño de la imagen no es 28x32.\n");
        return EXIT_FAILURE;
    } else {
        printf("Tamaño de la imagen correcto!\n");
    }

    /*
     * Cada pixel tiene 3 bytes, uno para cada color RGB.
     *
     * Si el pixel es blanco (255, 255, 255) guardamos un 0.
     * Si es negro (0, 0, 0) guardamos un -1.
     * Si es amarillo (255, 255, 0) guardamos un 1.
     * Si es verde (0 255 0) guardamos un 2
     * Si es cyan (0, 255, 255) guardamos un 6.
     * Si es azul (0, 0, 255) guardamos un 7.
     * Si es rojo (255, 0, 0) guardamos un 8.
     * Si es rosa (255, 0, 255) guardamos un 9.
     */
    
    // Creamos matriz para el mapa
    int **matrizMapa = (int **)malloc(32 * sizeof(int *));
    for (int i = 0; i < 32; i++){
        matrizMapa[i] = (int *)malloc(28 * sizeof(int));
    }
    
    // Contadores para verificar que exista solamente 1 pacman, y uno de cada fantasma.
    int contadorPacman = 0;
    int contadorF1 = 0;
    int contadorF2 = 0;
    int contadorF3 = 0;
    int contadorF4 = 0;

    // Las filas están guardadas de abajo hacia arriba, y el RGB está al revés (Está guardado como BGR)
    // (Comprobado viendo codigo hexadecimal de bpm ejemplo en https://hexed.it/)
    unsigned char pixelRGB[3];
    int pixelValue;
    for (int y = alto-1; y >= 0; y--) {
        for (int x = 0; x < ancho; x++) {
            fread(pixelRGB, sizeof(unsigned char), 3, file);
            
            if (pixelRGB[2] == 255 && pixelRGB[1] == 255 && pixelRGB[0] == 255) { // Blanco
                pixelValue = 0;              
            } else if (pixelRGB[2] == 0 && pixelRGB[1] == 0 && pixelRGB[0] == 0) { // Negro
                pixelValue = -1;
            } else if (pixelRGB[2] == 255 && pixelRGB[1] == 255 && pixelRGB[0] == 0) { // Amarillo
                pixelValue = 1;
            } else if (pixelRGB[2] == 0 && pixelRGB[1] == 255 && pixelRGB[0] == 0) { // Verde
                pixelValue = 2;
                contadorPacman++;
            } else if (pixelRGB[2] == 0 && pixelRGB[1] == 255 && pixelRGB[0] == 255) { // Cyan
                pixelValue = 6;
                contadorF1++;
            } else if (pixelRGB[2] == 0 && pixelRGB[1] == 0 && pixelRGB[0] == 255) { // Azul
                pixelValue = 7;
                contadorF2++;
            } else if (pixelRGB[2] == 255 && pixelRGB[1] == 0 && pixelRGB[0] == 0) { // Rojo
                pixelValue = 8;
                contadorF3++;
            } else if (pixelRGB[2] == 255 && pixelRGB[1] == 0 && pixelRGB[0] == 255) { // Rosa
                pixelValue = 9;
                contadorF4++;
            } else {
                printf("Error, color no reconocido en pixel (%d, %d), se finalizará el programa.\n", x, y);
                return EXIT_FAILURE;
            }

            // Verificación de pixeles
            // printf("Pixel (%d, %d): %d\n", x, y, pixelValue); // Esto es de debug para ver el comportamiento.
            matrizMapa[y][x] = pixelValue;
        }
    }

    if(contadorPacman != 1 || contadorF1 != 1 || contadorF2 != 1 || contadorF3 != 1 || contadorF4 != 1){
        printf("Error, debe haber un pacman y un fantasma de cada color en la imagen.\n");
        return EXIT_FAILURE;
    } else {
        printf("Imagen verificada correctamente!\n");
    }

    fclose(file);

    // Guardar matriz en archivo
    for (int i = 0; i < 32; i++){ 
        for (int j = 0; j < 28; j++){ 
            fprintf(mapa, "%d ", matrizMapa[i][j]);
        }
        fprintf(mapa, "\n");
    }

    printf("Mapa guardado correctamente!\n");
    printf("Ahora puedes usarlo en el juego, el mapa se llama mapaImagen.txt\n");
    
    fclose(mapa);  

    return 0;
}