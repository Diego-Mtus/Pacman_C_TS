
Para compilar este programa, usar gcc leerBMP.c -o leer
Para usar, usar leer <ubicación de bmp>

A modo de ejemplo, se tienen 2 dibujos en formato bpm en la carpeta ejemplodibujos.

GUÍA DE USO PARA CREAR IMAGENES MAPA:

0) Obs: Se utilizó Paint de Windows para crear imagenes, puede que otras aplicaciones codifiquen de manera
distinta el archivo bmp.
1) El archivo debe ser de 28 pixeles de ancho y 32 pixeles de alto.
2) Cada pixel será un Tile del mapa.
3) Se debe tener una paleta especifica de colores para dibujar mapa:

     * Si el pixel es blanco (255, 255, 255), será un espacio vacío.
     * Si es negro (0, 0, 0), será un bloque.
     * Si es amarillo (255, 255, 0), será una moneda.
     * Si es verde (0, 255, 0) será pacman.
     * Si es cyan (0, 255, 255), será el fantasma 1.
     * Si es azul (0, 0, 255), será el fantasma 2.
     * Si es rojo (255, 0, 0), será el fantasma 3.
     * Si es rosa (255, 0, 255), será el fantasma 4.

ADVERTENCIA: El lector de imagenes cuenta con verificación para esto, pero es muy importante que exista un y solamente un pixel para representar a pacman o a los fantasmas.

4) El archivo debe ser guardado en formato .BMP de 24 bits.
