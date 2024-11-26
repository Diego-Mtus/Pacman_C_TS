#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <SDL_mouse.h>
#include <SDL_image.h>
#include <string.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "definiciones.h"
#include "game.h"
#include "prints.h"



int main(int argc, char* argv[]){

    if(argc!=2) {
    printf("Ha olvidado ingresar mapa. El formato es: %s <ubicación del mapa> \n", argv[0]);
    return EXIT_FAILURE;
    }

    // Contador de frames, e inicialización de variables de FPS.
    printf("Iniciando juego...\n");
    int frameCounter = 0;
    int swap = 1;
    Uint32 frameStart;
	int frameTime;
    const int FPS = 60;
	const int frameDelay = 1000 / FPS;

    // Inicialización de SDL
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // Inicialización de sonidos
    Mix_Chunk* soundIntro = NULL;
    Mix_Chunk* chomp = NULL;
    Mix_Chunk* muerte = NULL;
    Mix_Chunk* victoria = NULL;
    

    // Inicializar SDL y todos los subsistemas
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, AnchoPantalla, AltoPantalla, SDL_WINDOW_RESIZABLE); // Crear ventana
        if(window == NULL){
            printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
            return EXIT_FAILURE;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // Crear renderer
        if (renderer == NULL) {
            fprintf(stderr, "Error al crear el renderer: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            return EXIT_FAILURE;
        }

        if(TTF_Init() == -1) { // Inicializar fuentes
            printf("Error con TTF_Init: %s\n", TTF_GetError());
            return EXIT_FAILURE;
        }
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { // Inicializar audio
            printf("Error con Mix_OpenAudio: %s\n", Mix_GetError());
            return EXIT_FAILURE;
        }

    } else {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Para que se vea mejor el juego al escalarlo
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    SDL_RenderSetLogicalSize(renderer, AnchoPantalla, AltoPantalla);

    // SECCIÓN DE CARGAR AUDIO

    Mix_Volume(-1, 36);

    soundIntro = cargarSonido("sonidos/intro.wav");
    chomp = cargarSonido("sonidos/chomp.wav");
    muerte = cargarSonido("sonidos/muerte.wav");
    victoria = cargarSonido("sonidos/victoria.wav");


    // Inicializamos mapa con malloc (-4 para dejar espacio para el score y vidas)

    int **mapa = (int **)malloc((CantidadVertical - 4) * sizeof(int *));
    for (int i = 0; i < CantidadVertical - 4; i++){
        mapa[i] = (int *)malloc(CantidadHorizontal * sizeof(int));
    }

    // Inicializamos objetos del juego

    // Letra para puntaje
    char texto[20] = "Puntos: 0";
    TTF_Font* font = TTF_OpenFont("texturas/ComicSans.ttf", 30);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, texto, (SDL_Color){255, 255, 255});
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    textRect.x = TileSize;
    textRect.y = AltoPantalla - 40;
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    SDL_FreeSurface(textSurface);

    // Letra para vidas
    char textoVidas[20] = "Vidas: 3";
    TTF_Font* fontVidas = TTF_OpenFont("texturas/ComicSans.ttf", 30);
    SDL_Surface *textSurfaceVidas = TTF_RenderText_Solid(fontVidas, textoVidas, (SDL_Color){255, 255, 255});
    SDL_Texture *textTextureVidas = SDL_CreateTextureFromSurface(renderer, textSurfaceVidas);
    SDL_Rect textRectVidas;
    textRectVidas.x = TileSize * 18;
    textRectVidas.y = AltoPantalla - 40;

    SDL_QueryTexture(textTextureVidas, NULL, NULL, &textRectVidas.w, &textRectVidas.h);
    SDL_FreeSurface(textSurfaceVidas);


    // Bloque
    SDL_Rect bloqueRect;
    SDL_Surface* tmpTextura = IMG_Load("texturas/bloque.png");
    SDL_Texture* bloque = SDL_CreateTextureFromSurface(renderer, tmpTextura);
    SDL_FreeSurface(tmpTextura);

    // Moneda
    SDL_Rect monedaRect;
    tmpTextura = IMG_Load("texturas/libremoneda.png");
    SDL_Texture* moneda = SDL_CreateTextureFromSurface(renderer, tmpTextura);
    SDL_FreeSurface(tmpTextura);


    // Pacman
    SDL_Rect pacmanRect;
    // Textura de pacman con boca abierta
    tmpTextura = IMG_Load("texturas/pacmanA.png");
    SDL_SetWindowIcon(window, tmpTextura);
    SDL_Texture* pacmanT_abierto = SDL_CreateTextureFromSurface(renderer, tmpTextura); // Aprovechando la instancia de cargar textura de pacman, se usa para el icono de la ventana
    SDL_FreeSurface(tmpTextura);
    // Textura de pacman con boca cerrada
    tmpTextura = IMG_Load("texturas/pacman.png");
    SDL_Texture* pacmanT_cerrado = SDL_CreateTextureFromSurface(renderer, tmpTextura);
    SDL_FreeSurface(tmpTextura);
    

    // Libre
    SDL_Rect vacioRect;
    tmpTextura = IMG_Load("texturas/libre.png");
    SDL_Texture* libre = SDL_CreateTextureFromSurface(renderer, tmpTextura);
    SDL_FreeSurface(tmpTextura);

    // Fantasma 1
    SDL_Rect fantasma1Rect;
    tmpTextura = IMG_Load("texturas/ghost1.png");
    SDL_Texture* fantasma1 = SDL_CreateTextureFromSurface(renderer, tmpTextura);
    SDL_FreeSurface(tmpTextura);

    int movRandomF1 = rand() % 4;
    int flip1 = SDL_FLIP_HORIZONTAL;

    // Fantasma 2
    SDL_Rect fantasma2Rect;
    tmpTextura = IMG_Load("texturas/ghost2.png");
    SDL_Texture* fantasma2 = SDL_CreateTextureFromSurface(renderer, tmpTextura);
    SDL_FreeSurface(tmpTextura);

    int movRandomF2 = rand() % 4;
    int flip2 = SDL_FLIP_NONE;

    // Fantasma 3
    SDL_Rect fantasma3Rect;
    tmpTextura = IMG_Load("texturas/ghost3.png");
    SDL_Texture* fantasma3 = SDL_CreateTextureFromSurface(renderer, tmpTextura);
    SDL_FreeSurface(tmpTextura);

    int movRandomF3 = rand() % 4;
    int flip3 = SDL_FLIP_NONE;

    // Fantasma 4
    SDL_Rect fantasma4Rect;
    tmpTextura = IMG_Load("texturas/ghost4.png");
    SDL_Texture* fantasma4 = SDL_CreateTextureFromSurface(renderer, tmpTextura);
    SDL_FreeSurface(tmpTextura);

    int movRandomF4 = rand() % 4;
    int flip4 = SDL_FLIP_HORIZONTAL;

    // SECCIÓN DE CARGAR MAPA
    char* path = argv[1];
    FILE *file = fopen(path, "r");

    if (file == NULL){
        printf("Error al abrir el archivo mapa. Debes asegurarte de que esté bien escrito (28 x 32 números)\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < CantidadVertical - 4; i++){ 
        for (int j = 0; j < CantidadHorizontal; j++){ 
            fscanf(file, "%d", &mapa[i][j]);
            tipo = mapa[i][j];
            Xdst = j * TileSize;
            Ydst = i * TileSize;
            switch (tipo) // 0 = vacio, 1 = moneda, 2 = pacman, -1 bloque, 6 fantasma1, 7 fantasma2, 8 fantasma3, 9 fantasma4
            {
            case 0: // Vacio
                ubicarRect(&vacioRect, Xdst, Ydst, TileSize, TileSize, renderer, libre);
                break;

            case 1: // Moneda
                monedastotal += 1;
                ubicarRect(&monedaRect, Xdst, Ydst, TileSize, TileSize, renderer, moneda);
                SDL_RenderCopy(renderer, moneda, NULL, &monedaRect);
                break;

            case 2: // Pacman               
                ubicarRect(&pacmanRect, Xdst + 1, Ydst + 1, PacmanSize, PacmanSize, renderer, pacmanT_abierto);

                // Guardar la ubicación inicial de pacman para el respawn
                xRespawn = Xdst + 1;
                yRespawn = Ydst + 1;
                mapa[i][j] = 0;
                break;
            
            case -1: // Bloque
                ubicarRect(&bloqueRect, Xdst, Ydst, TileSize, TileSize, renderer, bloque);
                break;

            case 6: // Fantasma 1
                ubicarRect(&fantasma1Rect, Xdst, Ydst, TileSize, TileSize, renderer, fantasma1);

                // Guardar la ubicación inicial de fantasma 1 para el respawn
                xRespawnF1 = Xdst;
                yRespawnF1 = Ydst;
                mapa[i][j] = 0;
                break;

            case 7: // Fantasma 2
                ubicarRect(&fantasma2Rect, Xdst, Ydst, TileSize, TileSize, renderer, fantasma2);

                // Guardar la ubicación inicial de fantasma 2 para el respawn
                xRespawnF2 = Xdst;
                yRespawnF2 = Ydst;
                mapa[i][j] = 0;
                break;

            case 8: // Fantasma 3
                ubicarRect(&fantasma3Rect, Xdst, Ydst, TileSize, TileSize, renderer, fantasma3);

                // Guardar la ubicación inicial de fantasma 3 para el respawn
                xRespawnF3 = Xdst;
                yRespawnF3 = Ydst;
                mapa[i][j] = 0;
                break;

            case 9: // Fantasma 4
                ubicarRect(&fantasma4Rect, Xdst, Ydst, TileSize, TileSize, renderer, fantasma4);

                // Guardar la ubicación inicial de fantasma 4 para el respawn
                xRespawnF4 = Xdst;
                yRespawnF4 = Ydst;
                mapa[i][j] = 0;
                break;
            }
        }
    }

    fclose(file);


    // Variables de manejo de eventos.
    const Uint8* key = SDL_GetKeyboardState(NULL);
    int quit = 0;
    SDL_Event event;


    // Reproducir sonido de inicio e imprimir instrucciones.
    Mix_PlayChannel(-1, soundIntro, 0);
    Mix_Volume(-1, 24);
    instrucciones(); 

    // !! !! !! !! !! !! !! !! !! !!
    // SECCIÓN DEL LOOP DEL JUEGO !!
    // !! !! !! !! !! !! !! !! !! !!

    while(!quit){

        // Tiempo de inicio del frame
        frameStart = SDL_GetTicks();
        frameCounter += 1;


        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT) quit = 1;

            // Eventos de teclado, además de mover a pacman, asigna dirección al azar para fantasmas
            if(event.type == SDL_KEYDOWN){
                if(key[SDL_SCANCODE_W] & direccion != 0) // Si se presiona W y no se está yendo hacia arriba (para evitar bug al mantener pulsada la tecla)
                {
                    direccion = 0;
                    // Asignar dirección al azar a los fantasmas
                    movRandom(&movRandomF1, &movRandomF2, &movRandomF3, &movRandomF4);
                }
                if(key[SDL_SCANCODE_S] & direccion != 1)
                {
                    direccion = 1;
                    movRandom(&movRandomF1, &movRandomF2, &movRandomF3, &movRandomF4);
                }
                if(key[SDL_SCANCODE_A] & direccion != 2) 
                {
                    direccion = 2;
                    movRandom(&movRandomF1, &movRandomF2, &movRandomF3, &movRandomF4);
                }
                if(key[SDL_SCANCODE_D] & direccion != 3)
                {
                    direccion = 3;
                    movRandom(&movRandomF1, &movRandomF2, &movRandomF3, &movRandomF4);
                }
                if(key[SDL_SCANCODE_ESCAPE])
                {
                    printf("Has salido del juego.\n"); 
                    quit = 1;  
                }

                if(key[SDL_SCANCODE_P]){
                    monedasActual = monedastotal;
                    monedasActualAnterior = monedastotal;
                } // Tecla para ganar automáticamente (Para debug)

                // IMPLEMENTACIÓN DE PAQUETE 1; ALMACENAMIENTO Y CAMBIO DE ESTADOS

                if(key[SDL_SCANCODE_G]) // Tecla para guardar el juego actual
                {
                    
                    FILE *save = fopen("save.txt", "w");
                    if(save == NULL){
                        printf("Error al guardar el juego\n");
                        break;
                    }
                    fprintf(save, "%d\n", Movimiento); // Guardar el estado de movimiento
                    fprintf(save, "%d\n", frameCounter); // Guardar el frame actual
                    fprintf(save, "%d\n", monedastotal); // Guardar la cantidad de monedas
                    fprintf(save, "%d\n", monedasActual); // Guardar la cantidad de monedas actuales
                    fprintf(save, "%d\n", monedasActualAnterior); // Guardar la cantidad de monedas anteriores
                    fprintf(save, "%d\n", vidas); // Guardar la cantidad de vidas  
                    //ubicación de pacman
                    fprintf(save, "%d\n", pacmanRect.x);
                    fprintf(save, "%d\n", pacmanRect.y);
                    fprintf(save, "%d\n", direccion); // Guardar la dirección de pacman
                    //ubicación de fantasma 1
                    fprintf(save, "%d\n", fantasma1Rect.x);
                    fprintf(save, "%d\n", fantasma1Rect.y);
                    fprintf(save, "%d\n", movRandomF1); // Guardar la dirección de fantasma 1
                    //ubicación de fantasma 2
                    fprintf(save, "%d\n", fantasma2Rect.x);
                    fprintf(save, "%d\n", fantasma2Rect.y);
                    fprintf(save, "%d\n", movRandomF2); // Guardar la dirección de fantasma 2
                    //ubicación de fantasma 3
                    fprintf(save, "%d\n", fantasma3Rect.x);
                    fprintf(save, "%d\n", fantasma3Rect.y);
                    fprintf(save, "%d\n", movRandomF3); // Guardar la dirección de fantasma 3
                    //ubicación de fantasma 4
                    fprintf(save, "%d\n", fantasma4Rect.x);
                    fprintf(save, "%d\n", fantasma4Rect.y);
                    fprintf(save, "%d\n", movRandomF4); // Guardar la dirección de fantasma 4
                    // Guardar el mapa
                    for (int i = 0; i < CantidadVertical - 4; i++){ 
                        for (int j = 0; j < CantidadHorizontal; j++){ 
                            fprintf(save, "%d ", mapa[i][j]);
                        }
                        fprintf(save, "\n");
                    }
                    fclose(save);
                    printf("Juego guardado correctamente!\n");
                }
                if(key[SDL_SCANCODE_C]) // Tecla para cargar el juego de save.txt
                { 
                    FILE *save = fopen("save.txt", "r");
                    if(save == NULL){
                        printf("Error al cargar el juego\n");
                        break;
                    }
                    fscanf(save, "%d", &Movimiento); // Cargar el estado de movimiento
                    fscanf(save, "%d", &frameCounter); // Cargar el frame actual
                    fscanf(save, "%d", &monedastotal); // Cargar la cantidad de monedas
                    fscanf(save, "%d", &monedasActual); // Cargar la cantidad de monedas actuales
                    fscanf(save, "%d", &monedasActualAnterior); // Cargar la cantidad de monedas anteriores
                    fscanf(save, "%d", &vidas); // Cargar la cantidad de vidas
                    //ubicación de pacman
                    fscanf(save, "%d", &pacmanRect.x);
                    fscanf(save, "%d", &pacmanRect.y);
                    fscanf(save, "%d", &direccion); // Cargar la dirección de pacman
                    //ubicación de fantasma 1
                    fscanf(save, "%d", &fantasma1Rect.x);
                    fscanf(save, "%d", &fantasma1Rect.y);
                    fscanf(save, "%d", &movRandomF1); // Cargar la dirección de fantasma 1
                    //ubicación de fantasma 2
                    fscanf(save, "%d", &fantasma2Rect.x);
                    fscanf(save, "%d", &fantasma2Rect.y);
                    fscanf(save, "%d", &movRandomF2); // Cargar la dirección de fantasma 2
                    //ubicación de fantasma 3
                    fscanf(save, "%d", &fantasma3Rect.x);
                    fscanf(save, "%d", &fantasma3Rect.y);
                    fscanf(save, "%d", &movRandomF3); // Cargar la dirección de fantasma 3
                    //ubicación de fantasma 4
                    fscanf(save, "%d", &fantasma4Rect.x);
                    fscanf(save, "%d", &fantasma4Rect.y);
                    fscanf(save, "%d", &movRandomF4); // Cargar la dirección de fantasma 4
                    // Cargar el mapa
                    for (int i = 0; i < CantidadVertical - 4; i++){ 
                        for (int j = 0; j < CantidadHorizontal; j++){ 
                            fscanf(save, "%d", &mapa[i][j]);
                        }
                    }
                    fclose(save);
                    printf("Juego cargado correctamente!\n");
                }
            }
        }

        // SECCIÓN DE ACTUALIZAR MOVIMIENTOS DE PACMAN Y FANTASMAS

        // Movimiento de pacman
        moverPacman(direccion, Movimiento, &pacmanRect, &angulo);

        // Movimiento de fantasma 1 al azar (Cyan)
        
        moverFantasma(movRandomF1, Movimiento, &fantasma1Rect, &flip1);

        // Movimiento de fantasma 2 al azar (Naranja)

        moverFantasma(movRandomF2, Movimiento, &fantasma2Rect, &flip2);

        // Movimiento de fantasma 3 al azar (Rojo)

        moverFantasma(movRandomF3, Movimiento, &fantasma3Rect, &flip3);

        // Movimiento de fantasma 4 al azar (Rosa)

        moverFantasma(movRandomF4, Movimiento, &fantasma4Rect, &flip4);

        // Si pacman o fantasma se sale del mapa, lo ubicamos al otro lado (Simulando portales) 
        
        simularPortales(&pacmanRect);
        simularPortales(&fantasma1Rect);
        simularPortales(&fantasma2Rect);
        simularPortales(&fantasma3Rect);
        simularPortales(&fantasma4Rect);


        //SECCIÓN DE RENDERIZADO
        SDL_RenderClear(renderer); // Limpiar pantalla

        // Renderizar mapa y aplicación de colisiones.
        for (int i = 0; i < CantidadVertical - 4; i++){ 
            for (int j = 0; j < CantidadHorizontal; j++){ 
                fscanf(file, "%d", &mapa[i][j]);
                tipo = mapa[i][j];
                Xdst = j * TileSize;
                Ydst = i * TileSize;
                switch (tipo) // 0 = vacio, 1 = moneda, -1 bloque
                {
                case 0: // Espacio libre
                    ubicarRect(&vacioRect, Xdst, Ydst, TileSize, TileSize, renderer, libre);
                    break;

                case 1: // Moneda
                    ubicarRect(&monedaRect, Xdst, Ydst, TileSize, TileSize, renderer, moneda);
                    if (checkCollision(pacmanRect, monedaRect)){
                        monedasActualAnterior += 1;
                        mapa[i][j] = 0;
                    }
                    break;

                case -1: // Bloque
                    ubicarRect(&bloqueRect, Xdst, Ydst, TileSize, TileSize, renderer, bloque);
                    if (checkCollision(pacmanRect, bloqueRect)){
                        ajustarColisionBloque(&pacmanRect, direccion, Movimiento);
                    }
                    if (checkCollision(fantasma1Rect, bloqueRect)){ // Colisión de fantasma 1 con bloque
                        ajustarColisionBloque(&fantasma1Rect, movRandomF1, Movimiento);
                        movRandomF1 = rand() % 4;
                    }
                    if (checkCollision(fantasma2Rect, bloqueRect)){ // Colisión de fantasma 2 con bloque
                        ajustarColisionBloque(&fantasma2Rect, movRandomF2, Movimiento);
                        movRandomF2 = rand() % 4;
                    }
                    if (checkCollision(fantasma3Rect, bloqueRect)){ // Colisión de fantasma 3 con bloque
                        ajustarColisionBloque(&fantasma3Rect, movRandomF3, Movimiento);
                        movRandomF3 = rand() % 4;
                    }
                    if (checkCollision(fantasma4Rect, bloqueRect)){ // Colisión de fantasma 4 con bloque
                        ajustarColisionBloque(&fantasma4Rect, movRandomF4, Movimiento);
                        movRandomF4 = rand() % 4;
                    }
                    break;
                }
            }
        }

        if (monedasActualAnterior != monedasActual){ // Lo hicimos así para evitar que se actualice el texto cada frame, y poder hacerlo fuera de la colisión de monedas
            monedasActual = monedasActualAnterior;
            sprintf(texto, "Puntos: %d", monedasActual*100);
            textSurface = TTF_RenderText_Solid(font, texto, (SDL_Color){255, 255, 255});
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
            SDL_FreeSurface(textSurface);
        }


        // Evento de victoria
        if (monedasActual == monedastotal){
            victoriaPrint();
            Mix_PlayChannel(-1, victoria, 0);
            SDL_Delay(3500);         
            quit = 1;    
        }

        // Colisión de pacman con fantasmas

        if (checkCollision(pacmanRect, fantasma1Rect) || checkCollision(pacmanRect, fantasma2Rect) 
        || checkCollision(pacmanRect, fantasma3Rect) || checkCollision(pacmanRect, fantasma4Rect)){

            // Restar vidas
            vidas -= 1;
            sprintf(textoVidas, "Vidas: %d", vidas);
            textSurfaceVidas = TTF_RenderText_Solid(fontVidas, textoVidas, (SDL_Color){255, 255, 255});
            textTextureVidas = SDL_CreateTextureFromSurface(renderer, textSurfaceVidas);
            SDL_QueryTexture(textTextureVidas, NULL, NULL, &textRectVidas.w, &textRectVidas.h);
            SDL_FreeSurface(textSurfaceVidas);

            // Sonido de muerte
            Mix_PlayChannel(-1, muerte, 0);
            SDL_Delay(1800);

            if (vidas == 0){
                gameOver();
                quit = 1;
            }
            // Reposicionar a pacman y fantasmas
            pacmanRect.x = xRespawn;
            pacmanRect.y = yRespawn;
            fantasma1Rect.x = xRespawnF1;
            fantasma1Rect.y = yRespawnF1;
            fantasma2Rect.x = xRespawnF2;
            fantasma2Rect.y = yRespawnF2;
            fantasma3Rect.x = xRespawnF3;
            fantasma3Rect.y = yRespawnF3;
            fantasma4Rect.x = xRespawnF4;
            fantasma4Rect.y = yRespawnF4;
        }

        // Renderizar pacman

        // Añadimos este contador para que cada 20 frames, pacman cambie de sprite y suene el chomp
        if(frameCounter % 20 == 0 && Movimiento == 1)
        {
            Mix_PlayChannel(-1, chomp, 0);
            swap *= -1; 
        }

        if(swap == 1){
            SDL_RenderCopyEx(renderer, pacmanT_abierto, NULL, &pacmanRect, angulo, NULL, SDL_FLIP_NONE);
        } else {
            SDL_RenderCopyEx(renderer, pacmanT_cerrado, NULL, &pacmanRect, angulo, NULL, SDL_FLIP_NONE);
        }
            
        // Renderizar fantasmas (flip sirve para que fantasmas miren a izquierda o derecha)
        SDL_RenderCopyEx(renderer, fantasma1, NULL, &fantasma1Rect, 0, NULL, flip1);
        SDL_RenderCopyEx(renderer, fantasma2, NULL, &fantasma2Rect, 0, NULL, flip2);
        SDL_RenderCopyEx(renderer, fantasma3, NULL, &fantasma3Rect, 0, NULL, flip3); 
        SDL_RenderCopyEx(renderer, fantasma4, NULL, &fantasma4Rect, 0, NULL, flip4);
        
        // Renderizar texto
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_RenderCopy(renderer, textTextureVidas, NULL, &textRectVidas);

        //Actualizar renderer para mostrar en pantalla
        SDL_RenderPresent(renderer);

        // Para que frameCounter no se haga muy grande
        if(frameCounter == 6002){
            frameCounter = 242;
        }

        // Limitar fps a 60
        frameTime = SDL_GetTicks() - frameStart; // Tiempo que tomo un frame
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		} // Si el tiempo que tomo un frame es menor al tiempo que deberia tomar, esperar la diferencia


        if(frameCounter == 240){ // Esto para que se reproduzca la intro igual que en el juego original, donde se escucha el sonido de inicio y luego se mueve pacman
            Movimiento = 1;
        }
    }

    // Liberar memoria
    for (int i = 0; i < CantidadVertical - 4; i++){
        free(mapa[i]);
    }
    free(mapa);
    Mix_FreeChunk(soundIntro); 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
    
};