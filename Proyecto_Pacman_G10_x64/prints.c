#include "prints.h"

// Este archivo se encarga de manejar las impresiones en consola.

// Función para imprimir las instrucciones del juego.
void instrucciones(){
    printf("\nInstrucciones:\n");
    printf("Muevete con las teclas W, A, S, D.\n");
    printf("Come todas las monedas para ganar.\n");
    printf("Evita a los fantasmas.\n");
    printf("Si te atrapan, pierdes una vida.\n");
    printf("Si pierdes todas tus vidas, pierdes.\n");
    printf("Presiona ESC para salir.\n");
    printf("Presiona G para guardar el juego.\n");
    printf("Presiona C para cargar el juego.\n");
}

// Función para imprimir Game Over.
void gameOver(){
    printf("\n");
    printf("  _____                         ____                 \n");
    printf(" / ____|                       / __ \\                \n");
    printf("| |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ \n");
    printf("| | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|\n");
    printf("| |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   \n");
    printf(" \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   \n");
    printf("\n");
}

// Función para imprimir victoria.
void victoriaPrint(){
    printf("\n");
    printf(" __     ______  _    _  __          _______ _   _ \n");
    printf(" \\ \\   / / __ \\| |  | | \\ \\        / /_   _| \\ | |\n");
    printf("  \\ \\_/ / |  | | |  | |  \\ \\  /\\  / /  | | |  \\| |\n");
    printf("   \\   /| |  | | |  | |   \\ \\/  \\/ /   | | | . ` |\n");
    printf("    | | | |__| | |__| |    \\  /\\  /   _| |_| |\\  |\n");
    printf("    |_|  \\____/ \\____/      \\/  \\/   |_____|_| \\_|\n");
    printf("\n");
}
