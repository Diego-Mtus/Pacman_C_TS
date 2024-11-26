#include <SDL.h>
#include <stdbool.h>
#include <SDL_mixer.h>
#include <stdio.h>


// Prototipo de función de colisiones:
// Recibe los 2 rectangulos a probar colisión
bool checkCollision(SDL_Rect a, SDL_Rect b);

// Prototipo de función de cargar y verificar sonidos:
// Recibe la ruta del archivo de sonido
Mix_Chunk* cargarSonido(const char* path);

// Prototipo de ubicar Rect del mapa y renderizarlo:
// Recibe un rectángulo, y las coordenadas x, y, ancho y alto, el renderer y la textura a renderizar.
void ubicarRect(SDL_Rect* rect, int x, int y, int w, int h, SDL_Renderer* renderer, SDL_Texture* text);

// Prototipo de función de movimiento de pacman:
// Recibe la dirección, el movimiento, el Rect de pacman y puntero a angulo.
void moverPacman(int direccion, int Movimiento, SDL_Rect* pacmanRect, double* angulo);

// Prototipo de función de movimiento al azar de los fantasmas:
// Recibe el int random, el movimiento, el Rect del fantasma y puntero a flip
void moverFantasma(int movRandom, int Movimiento, SDL_Rect* fantasmaRect, int* flip);

// Prototipo de función de simular portales horizontales:
// Recibe solamente Rect.
void simularPortales(SDL_Rect* rect);

// Prototipo de función de ajuste de colisiones con bloques:
// Recibe Rect, direccion y movimiento.
void ajustarColisionBloque(SDL_Rect* rect, int direccion, int Movimiento);

// Prototipo de función que asigna valores aleatorios a los movimientos de los fantasmas:
// Recibe punteros a los 4 movimientos random.
void movRandom(int *movRandomF1, int *movRandomF2, int *movRandomF3, int *movRandomF4);


