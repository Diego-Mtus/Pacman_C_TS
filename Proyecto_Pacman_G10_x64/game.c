#include "game.h"


bool checkCollision(SDL_Rect a, SDL_Rect b){
    // Los lados de los rectangulos
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // Calcular los lados del rectangulo A
    // Hacemos la colision del Rect A más pequeña para que no haya problemas con esquinas al moverse.
    leftA = a.x + 1;
    rightA = a.x + a.w - 1;
    topA = a.y + 1;
    bottomA = a.y + a.h - 1;

    // Calcular los lados del rectangulo B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    // Si alguno de los lados del rectangulo A está fuera del rectangulo B, no hay colisión
    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;

    // Si no se cumple ninguna de las condiciones anteriores, hay colisión
    return true;
}


// Función para cargar sonidos, recibe la ruta del archivo de sonido, y si hay un error, imprime el error y termina el programa.
Mix_Chunk* cargarSonido(const char* path) {
    Mix_Chunk* sonido = Mix_LoadWAV(path); 
    if (sonido == NULL) {
        printf("Error al cargar sonido: %s\n", Mix_GetError());
        exit(EXIT_FAILURE);
    }
    return sonido;
}

// Función para ubicar un rectángulo en el mapa, recibe un rectángulo, y las coordenadas x, y, ancho y alto, y lo renderiza.
void ubicarRect(SDL_Rect* rect, int x, int y, int w, int h, SDL_Renderer* renderer, SDL_Texture* text){
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    SDL_RenderCopy(renderer, text, NULL, rect);
}

// Función para mover a pacman, recibe la dirección, el movimiento y el rectángulo de pacman, junto a puntero para rotar angulo. ( 0 = arriba, 1 = abajo, 2 = izquierda, 3 = derecha)
void moverPacman(int direccion, int Movimiento, SDL_Rect* pacmanRect, double *angulo){
    switch (direccion)
    {
    case 0: // Arriba
        pacmanRect->y -= Movimiento;
        *angulo = 270;
        break;
    case 1: // Abajo
        pacmanRect->y += Movimiento;
        *angulo = 90;
        break;
    case 2: // Izquierda
        pacmanRect->x -= Movimiento;
        *angulo = 180;
        break;
    case 3: // Derecha
        pacmanRect->x += Movimiento;
        *angulo = 0;
        break;
    }
}


// Función para mover fantasmas al azar, recibe dirección random, movimiento, rectángulo del fantasma y puntero a flip.
void moverFantasma(int movRandom, int Movimiento, SDL_Rect* fantasmaRect, int *flip){
    switch (movRandom)
    {
    case 0: // Arriba
        fantasmaRect->y -= Movimiento;
        break;
    case 1: // Abajo
        fantasmaRect->y += Movimiento;
        break;
    case 2: // Izquierda
        fantasmaRect->x -= Movimiento;
        *flip = SDL_FLIP_HORIZONTAL;
        break;
    case 3: // Derecha
        fantasmaRect->x += Movimiento;
        *flip = SDL_FLIP_NONE;
        break;
    }
}

// Función para simular portales, si pacman o fantasma se sale del mapa, lo ubicamos al otro lado
int funTileSize = 16;
int funAnchoPantalla = 448; // 28 * 16
int funAltoPantalla = 512; // 32 * 16
void simularPortales(SDL_Rect* rect){
    if (rect->x >= funAnchoPantalla){
        rect->x = 0;
    } else if (rect->x < 0){
        rect->x = funAnchoPantalla - funTileSize;
    } else if (rect->y >= funAltoPantalla - funTileSize){
        rect->y = 0;
    } else if (rect->y < 0){
        rect->y = funAltoPantalla - funTileSize;
    }
}

// Función para ajustar colisiones con bloques, recibe rectángulo, dirección y movimiento.
// Si choca con bloque en cierta dirección, lo mueve en sentido contrario
// 0 = arriba, 1 = abajo, 2 = izquierda, 3 = derecha
void ajustarColisionBloque(SDL_Rect* rect, int direccion, int Movimiento){
    switch(direccion){
        case 0: // Choca arriba, mover abajo
            rect->y += Movimiento;
            break;
        case 1: // Choca abajo, mover arriba
            rect->y -= Movimiento;
            break;
        case 2: // Choca izquierda, mover derecha
            rect->x += Movimiento;
            break;
        case 3: // Choca derecha, mover izquierda
            rect->x -= Movimiento;
            break;
    }
}


// Función que asigna valores aleatorios a los movimientos de los fantasmas, recibe punteros a los 4 movimientos random.
void movRandom(int *movRandomF1, int *movRandomF2, int *movRandomF3, int *movRandomF4){
    *movRandomF1 = rand() % 4;
    *movRandomF2 = rand() % 4;
    *movRandomF3 = rand() % 4;
    *movRandomF4 = rand() % 4;
}



