#define TileSize 16
#define PacmanSize 14
#define CantidadHorizontal 28
#define CantidadVertical 36
#define AnchoPantalla TileSize * CantidadHorizontal
#define AltoPantalla TileSize * CantidadVertical


// Variables de juego iniciales


int tipo; // Tipo de objeto en el mapa (moneda, bloque, vacio)
int Xdst;
int Ydst;
int xRespawn; // Respawn de pacman si choca con fantasma
int yRespawn;
int xRespawnF1; // Respawn de fantasma 1
int yRespawnF1;
int xRespawnF2; // Respawn de fantasma 2
int yRespawnF2;
int xRespawnF3; // Respawn de fantasma 3
int yRespawnF3;
int xRespawnF4; // Respawn de fantasma 4
int yRespawnF4;

int monedastotal = 0; // Cantidad de monedas en el mapa, se inicializa en 0
int monedasActual = 0;
int monedasActualAnterior = 0;

int direccion = 4; // Dirección de pacman (0 = arriba, 1 = abajo, 2 = izquierda, 3 = derecha, 4 = quieto)
double angulo = 0; // Ángulo de rotación de pacman
int Movimiento = 0;
int vidas = 3; // Vidas de pacman