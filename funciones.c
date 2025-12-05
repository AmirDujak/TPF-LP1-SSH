#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "funciones.h"

static int filaDisponible(const char tablero[ROWS][COLS], int columna);
static bool hayLineaDeCuatro(char matriz[ROWS][COLS], char ficha);
static bool esJugadaGanadora(char matriz[ROWS][COLS], int columna, char ficha);
static int puntuarVentana(const char ventana[4], char fichaIA);
static int puntuarTablero(char matriz[ROWS][COLS], char fichaIA);

void limpiarBuffer(void) {
    char aux;
    while ((aux = getchar()) != '\n' && aux != EOF);
}

int generarRandom(void) { //Genera un número aleatorio entre el mínimo y el máximo (en este caso, 1 y 7 respesctivamente)
    int min = 1;
    int max = 4;
    int randomInRange = (rand() & (max - min + 1)) + min;
    return randomInRange;
}

void leerCadena(char x[MAX_SIZE]) {
    int i = 0;
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        x[i] = (char)ch;
        i++;
    }
    x[i] = '\0';
}

void reglas(void) {
    //Por ahora solamente lo importante
    printf("Para colocar una ficha se tendra que elegir la columna en la que se desea colocar la ficha\n");
    printf("La ficha se colocara en la primera fila con un espacio vacio en dicha columna, verificando desde abajo hacia arriba\n");
    printf("En caso de que no haya una fila libre, se te pedira que ingreses otra columna\n");
}

int elegirTurno(void) {
    int temp;
    printf("Quién empieza la partida?\n1 para rojo, 2 para azul, 3 para aleatorio: ");
    scanf("%d", &temp);
    while (!(temp == 1 || temp == 2 || temp == 3)) { //Sacar cuando se implemente el random
        limpiarBuffer();
        printf("Favor elija 1 para rojo, 2 para azul o 3 para aleatorio: ");
        scanf("%d", &temp);
    }
    while (temp == 3) { //Sacar cuando se implemente el random
        limpiarBuffer();
        printf("Función no implementada actualmente. Favor elija 1 para rojo o 2 para azul: ");
        scanf("%d", &temp);
    }
    if (temp == 3) {
        printf("TODAVÍA NO ESTÁ IMPLEMENTADO!!\n");
        //Acá haría un cálculo random
    } else {
        return temp;
    }
    return 0;
}

void dibujarTurno(int *turno, char X[MAX_SIZE], char Y[MAX_SIZE]) {
    if (*turno == 1) {
        printf("   Es el turno de %s\n", X);
    } else {
        printf("   Es el turno de %s\n", Y);
    }
}

//Se inicializa el tablero con puntos
void iniciarTablero(char b[ROWS][COLS], bool *inicializado) {
    if (*inicializado) {
        return;
    } else {
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                b[r][c] = '.';
            }
        }
        *inicializado = true;
    }
}

//! LEER DESPUES
static int filaDisponible(const char tablero[ROWS][COLS], int columna) {
    if (columna < 0 || columna >= COLS) {
        return -1;
    }
    for (int fila = ROWS - 1; fila >= 0; fila--) {
        if (tablero[fila][columna] == '.' ) {
            return fila;
        }
    }
    return -1;
}

//! LEER DESPUES
static bool hayLineaDeCuatro(char matriz[ROWS][COLS], char ficha) {
    // Horizontal y vertical
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (matriz[r][c] != ficha) {
                continue;
            }
            if (c + 3 < COLS &&
                matriz[r][c + 1] == ficha &&
                matriz[r][c + 2] == ficha &&
                matriz[r][c + 3] == ficha) {
                return true;
            }
            if (r + 3 < ROWS &&
                matriz[r + 1][c] == ficha &&
                matriz[r + 2][c] == ficha &&
                matriz[r + 3][c] == ficha) {
                return true;
            }
            if (r + 3 < ROWS && c + 3 < COLS &&
                matriz[r + 1][c + 1] == ficha &&
                matriz[r + 2][c + 2] == ficha &&
                matriz[r + 3][c + 3] == ficha) {
                return true;
            }
            if (r + 3 < ROWS && c - 3 >= 0 &&
                matriz[r + 1][c - 1] == ficha &&
                matriz[r + 2][c - 2] == ficha &&
                matriz[r + 3][c - 3] == ficha) {
                return true;
            }
        }
    }
    return false;
}

//! LEER DESPUES
static bool esJugadaGanadora(char matriz[ROWS][COLS], int columna, char ficha) {
    int fila = filaDisponible(matriz, columna);
    if (fila < 0) {
        return false;
    }
    char copia[ROWS][COLS];
    memcpy(copia, matriz, sizeof(copia));
    copia[fila][columna] = ficha;
    return hayLineaDeCuatro(copia, ficha);
}

//! LEER DESPUES
static int puntuarVentana(const char ventana[4], char fichaIA) {
    char rival = (fichaIA == 'X') ? 'O' : 'X';
    int cuentaIA = 0, cuentaRival = 0, vacios = 0;
    for (int i = 0; i < 4; i++) {
        if (ventana[i] == fichaIA) cuentaIA++;
        else if (ventana[i] == rival) cuentaRival++;
        else if (ventana[i] == '.') vacios++;
    }

    if (cuentaIA == 4) return 10000;
    if (cuentaIA == 3 && vacios == 1) return 120;
    if (cuentaIA == 2 && vacios == 2) return 20;
    if (cuentaRival == 3 && vacios == 1) return -150;
    if (cuentaRival == 2 && vacios == 2) return -40;
    return 0;
}

//! LEER DESPUES
static int puntuarTablero(char matriz[ROWS][COLS], char fichaIA) {
    int puntaje = 0;
    int columnaCentral = COLS / 2;
    for (int r = 0; r < ROWS; r++) {
        if (matriz[r][columnaCentral] == fichaIA) {
            puntaje += 6;
        }
    }

    // Horizontal
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c <= COLS - 4; c++) {
            char ventana[4] = {matriz[r][c], matriz[r][c + 1], matriz[r][c + 2], matriz[r][c + 3]};
            puntaje += puntuarVentana(ventana, fichaIA);
        }
    }

    // Vertical
    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r <= ROWS - 4; r++) {
            char ventana[4] = {matriz[r][c], matriz[r + 1][c], matriz[r + 2][c], matriz[r + 3][c]};
            puntaje += puntuarVentana(ventana, fichaIA);
        }
    }

    // Diagonal principal
    for (int r = 0; r <= ROWS - 4; r++) {
        for (int c = 0; c <= COLS - 4; c++) {
            char ventana[4] = {matriz[r][c], matriz[r + 1][c + 1], matriz[r + 2][c + 2], matriz[r + 3][c + 3]};
            puntaje += puntuarVentana(ventana, fichaIA);
        }
    }

    // Diagonal inversa
    for (int r = 0; r <= ROWS - 4; r++) {
        for (int c = 3; c < COLS; c++) {
            char ventana[4] = {matriz[r][c], matriz[r + 1][c - 1], matriz[r + 2][c - 2], matriz[r + 3][c - 3]};
            puntaje += puntuarVentana(ventana, fichaIA);
        }
    }

    return puntaje;
}

void menu(int *juego, char X[MAX_SIZE], char Y[MAX_SIZE], int *turno) {
    //Último voy a cambiar el formato para que sea más legible. Ahora quiero que funcione nomás
    int gameMode, continuar, turnoIA;
    printf("\t\t\033[33m=================================\033[0m\n");
    printf("\t\t\033[33m    Bienvenido a 4 en raya!!!\033[0m\n");
    reglas();
    puts("");
    printf("Seleccione el modo de juego:\n1: PvP\n2: PvE\n3: EvE\n");
    scanf("%d", &gameMode);
    while (gameMode > 3 || gameMode < 1) {
        limpiarBuffer();
        printf("Favor elija una opción válida: ");
        scanf("%d", &gameMode);
    }
    limpiarBuffer();
    if (gameMode == 1) { //Este es en caso de que el usuario elija PvP
        printf("Has seleccionado el modo Player vs Player!\n"); //Se ingresan los nombres del jugador 1 y 2. Luego se elige quien empieza.
        printf("Escriba el nombre del jugador 1: ");
        leerCadena(X);
        while (X[0] == '\0') {
            printf("Favor introduzca un nombre para el jugador 1: ");
            leerCadena(X);
        }
        printf("Escriba el nombre del jugador 2: ");
        leerCadena(Y);
        while (Y[0] == '\0') {
            printf("Favor introduzca un nombre para el jugador 2: ");
            leerCadena(Y);
        }
        *turno = elegirTurno();
    } else if (gameMode == 2) { //Este es en caso de que el usuario elija PvE
        printf("Has seleccionado el modo Player vs Environment!\n"); //Se ingresa el nombre del jugador 1. Luego el jugador elige si empieza él o la máquina
        printf("Escriba el nombre del jugador 1: ");
        leerCadena(X);
        *turno = elegirTurno();
    } else { //Este es en caso de que el usuario elija EvE
        printf("Has seleccionado el modo Environment vs Environment!\n"); //No se ingresan los nombres
        turnoIA = rand(); //Genera un número random para el turno
        if (turnoIA % 2 == 0) { //Si el resto de dividir el número entre 2 es 0, la IA 1 empieza. Caso contrario empieza la IA 2.
            *turno = 1;
        } else {
            *turno = 2;
        }
    }
    printf("\t\t\033[33m=================================\033[0m\n");


    *juego = gameMode;
}

//Función que imprime el tablero siempre que se le llama
void imprimirTablero(char b[ROWS][COLS], int *estado) {
    puts("");
    //Encabezado con índices de las columnas
    printf("   ");
    for(int c = 0; c < COLS; ++c) {
        printf(" %2d ", c+1);
    }
    puts("");

    //Separador superior
    printf("    ");
    for (int c = 0; c < COLS; ++c) {
        printf("--- ");
    }
    puts("");

    for (int r = 0; r < ROWS; ++r) {
        printf("%2d |", ROWS-r);
        for (int c = 0; c < COLS; ++c) {
            if (b[r][c] == 'X' || b[r][c] == 'x') {
                printf("\033[31m X \033[0m|");
            } else if (b[r][c] == 'O' || b[r][c] == 'o') {
                printf("\033[34m O \033[0m|");
            } else {
                printf(" %c |", b[r][c]);
            }
        }
        puts("");
        printf("    ");
        for (int c = 0; c < COLS; ++c) {
            printf("--- ");
        }
        puts("");
    }

    if (*estado == 0) {
        printf("   La partida sigue en juego\n");
    }
}


int elegirUbicacionDeFicha(int *turno, char matriz[ROWS][COLS], int *estado, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int modoDeJuego, int *columnaAColocar) {
    int columna = *columnaAColocar;
    int fila = 0;
    int walter = *estado;
    do {
        if (matriz[0][columna] == 'X' || matriz[0][columna] == 'O') {
           printf("Error! La columna esta llena!\n");
           *columnaAColocar = -1;
           return 0;
        }
        for (int i = 5; i >= 0; i--) {
            if (matriz[i][columna] == 'X' || matriz[i][columna] == 'O') {
                continue;
            } else {
                fila = i;
                if (*turno == 0) {
                    matriz[fila][columna] = 'X';
                    *turno = 1;
                    *columnaAColocar = -1;
                    return 0;
                } else if (*turno == 1) {
                    matriz[fila][columna] = 'O';
                    *turno = 0;
                    *columnaAColocar = -1;
                    return 0;
                }
            }
        }
    } while (1);
    return 0;
}

void colocarUbicacionIA(int *turno, char matriz[ROWS][COLS], int *estado, int *lleno) {
    int fila = 0;
    int walter = *estado;
    if (*lleno == 7) {
        *estado = 3;
        return;
    }
    do {
        int fichaIA = generarRandom();
        int columna = fichaIA;
        for (int i = 5; i >= 0; i--) {
            if (matriz[0][columna-1] == 'X' || matriz[0][columna-1] == 'O') {
                break;
            } else if (matriz[i][columna-1] == 'X' || matriz[i][columna-1] == 'O') {
                continue;
            } else {
                fila = i;
                if (*turno == 1) {
                    matriz[fila][columna-1] = 'X';
                    *turno = 2;
                    return;
                } else if (*turno == 2) {
                    matriz[fila][columna-1] = 'O';
                    *turno = 1;
                    return;
                }
            }
        }
    } while (1);
    for (int i = 0; i < 6; i++) {
        if (matriz[0][i] == 'X' || matriz[0][i] == 'O') {
            *lleno = *lleno + 1;
        }
    }
}

int seleccionarColumnaIA(char tablero[ROWS][COLS], int turno) {
    char fichaIA = (turno == 0) ? 'X' : 'O';
    char rival = (fichaIA == 'X') ? 'O' : 'X';
    int ordenColumnas[COLS] = {3, 2, 4, 1, 5, 0, 6};

    for (int i = 0; i < COLS; i++) {
        int col = ordenColumnas[i];
        if (esJugadaGanadora(tablero, col, fichaIA)) {
            return col;
        }
    }

    for (int i = 0; i < COLS; i++) {
        int col = ordenColumnas[i];
        if (esJugadaGanadora(tablero, col, rival)) {
            return col;
        }
    }

    int mejorColumna = -1;
    int mejorPuntaje = -1000000;
    for (int i = 0; i < COLS; i++) {
        int col = ordenColumnas[i];
        int fila = filaDisponible(tablero, col);
        if (fila < 0) {
            continue;
        }
        char copia[ROWS][COLS];
        memcpy(copia, tablero, sizeof(copia));
        copia[fila][col] = fichaIA;
        int puntaje = puntuarTablero(copia, fichaIA);
        if (puntaje > mejorPuntaje) {
            mejorPuntaje = puntaje;
            mejorColumna = col;
        }
    }

    return mejorColumna;
}

void verificarVictoria(char matriz[ROWS][COLS], int *estado, int *lleno1, int *lleno2, int *lleno) {
    *lleno = 0;
    for (int i = 0; i < COLS; i++) {
        if (matriz[0][i] == 'X' || matriz[0][i] == 'O') {
            *lleno = *lleno + 1;
        }
    }
    *lleno1 = 0;
    *lleno2 = 0;
    *estado = 0;

    if (hayLineaDeCuatro(matriz, 'X')) {
        *estado = 1;
        return;
    }
    if (hayLineaDeCuatro(matriz, 'O')) {
        *estado = 2;
        return;
    }

    if (*lleno == COLS) {
        *estado = 3;
    }
}

void guardarPartida(char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int modoDeJuego, int turno, int estadoPartida, char tablero[ROWS][COLS]) {
    int partidaAGuardar;
    FILE *fp;
    printf("Ingrese el número de la partida a guardar (1, 2 o 3): ");
    scanf("%d", &partidaAGuardar);
    while (partidaAGuardar < 1 || partidaAGuardar > 3) {
        printf("Ingrese un numero del 1 al 3: ");
        scanf("%d", &partidaAGuardar);
    }

    char contenidoFila[COLS];

    if (partidaAGuardar == 1) {
        fp = fopen("save1.txt", "w");
        if (fp == NULL) {printf("No se pudo guardar en el archivo 1\n"); return;}
        fputs(jugador1, fp);
        fprintf(fp, "\n");
        fputs(jugador2, fp);
        fprintf(fp, "\n");
        fprintf(fp, "%d\n%d\n%d\n", modoDeJuego, turno, estadoPartida);
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                contenidoFila[j] = tablero[i][j];
            }
            fputs(contenidoFila, fp);
            fprintf(fp, "\n");
        }
        printf("Partida guardada en el archivo 1!\n");
        fclose(fp);
    } else if (partidaAGuardar == 2) {
        fp = fopen("save2.txt", "w");
        if (fp == NULL) {printf("No se pudo guardar en el archivo 2\n"); return;}
        fputs(jugador1, fp);
        fprintf(fp, "\n");
        fputs(jugador2, fp);
        fprintf(fp, "\n");
        fprintf(fp, "%d\n%d\n%d\n", modoDeJuego, turno, estadoPartida);
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                contenidoFila[j] = tablero[i][j];
            }
            fputs(contenidoFila, fp);
            fprintf(fp, "\n");
        }
        printf("Partida guardada en el archivo 2!\n");
        fclose(fp);
    } else {
        fp = fopen("save3.txt", "w");
        if (fp == NULL) {printf("No se pudo guardar en el archivo 3\n"); return;}
        fputs(jugador1, fp);
        fprintf(fp, "\n");
        fputs(jugador2, fp);
        fprintf(fp, "\n");
        fprintf(fp, "%d\n%d\n%d\n", modoDeJuego, turno, estadoPartida);
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                contenidoFila[j] = tablero[i][j];
            }
            fputs(contenidoFila, fp);
            fprintf(fp, "\n");
        }
        printf("Partida guarada en el archivo 3!\n");
        fclose(fp);
    }

    return;
}

int cargarPartida(char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int *modoDeJuego, int *turno, int *estadoPartida, char tablero[ROWS][COLS]) {
    int partidaACargar = 1;
    FILE *fp;
    //printf("Ingrese el numero de partida a cargar del 1 al 3 (-1 para cancelar): ");
    //scanf("%d", &partidaACargar);
    if (partidaACargar == -1) {return -1;}
    while (partidaACargar < 1 || partidaACargar > 3) {
        printf("Ingrese un numero del 1 al 3 (-1 para cancelar): ");
        scanf("%d", &partidaACargar);
        if (partidaACargar == -1) {return -1;}
    }

    char contenidoFila[COLS];
    int a = 0;
    char c;

    if (partidaACargar == 1) {
        fp = fopen("save1.txt", "r");
        if (fp == NULL) {printf("No se pudo cargar el archivo 1\n"); return -1;}
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {a=0; break;}
            jugador1[a++] = c;
        }
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {break;}
            jugador2[a++] = c;
        }
        fscanf(fp, "%d\n%d\n%d\n", modoDeJuego, turno, estadoPartida);
        int i = 0; 
        int j = 0;
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {
                i++;
                j=0;
                continue;
            }
            tablero[i][j++] = c;
        }
        printf("Se cargo la partida del archivo 1!\n");
        fclose(fp);
        sleep(2);
    } else if (partidaACargar == 2) {
        fp = fopen("save1.txt", "r");
        if (fp == NULL) {printf("No se pudo cargar el archivo 2\n"); return -1;}
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {break;}
            jugador1[a++] = c;
        }
        limpiarBuffer();
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {break;}
            jugador2[a++] = c;
        }
        fscanf(fp, "%d\n%d\n%d\n", modoDeJuego, turno, estadoPartida);
        int i = 0; 
        int j = 0;
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {
                i++;
                continue;
            }
            tablero[i][j++] = c;
        }
        printf("Se cargo la partida del archivo 2!\n");
        fclose(fp);
        sleep(2);
    } else {
        fp = fopen("save3.txt", "r");
        if (fp == NULL) {printf("No se pudo cargar el archivo 3\n"); return -1;}
        while ((c = fgetc(fp)) != EOF) { 
            if (c == '\n') {break;}
            jugador1[a++] = c;
        }
        limpiarBuffer();
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {break;}
            jugador2[a++] = c;
        }
        fscanf(fp, "%d\n%d\n%d\n", modoDeJuego, turno, estadoPartida);
        int i = 0; 
        int j = 0;
        while ((c = fgetc(fp)) != EOF) {
            if (c == '\n') {
                i++;
                continue;
            }
            tablero[i][j++] = c;
        }
        printf("Se cargo la partida del archivo 1!\n");
        fclose(fp);
        sleep(2);
    }


    return 0;
}

//!
static void recalcularPorcentaje(StatsJugador *j) {
    if (j->partidasJugadas > 0) {
        j->porcentajeVictorias = (100.0f * j->partidasGanadas) / j->partidasJugadas;
    } else {
        j->porcentajeVictorias = 0.0f;
    }
}

static void limpiarCaraACara(CaraACara caraACara[MAX_JUGADORES][MAX_JUGADORES]) {
    for (int i = 0; i < MAX_JUGADORES; i++) {
        for (int j = 0; j < MAX_JUGADORES; j++) {
            caraACara[i][j].w = 0;
            caraACara[i][j].l = 0;
            caraACara[i][j].e = 0;
        }
    }
}

static int buscarJugadorPorNombre(StatsJugador jugadores[], int cantidad, const char *nombre) {
    for (int i = 0; i < cantidad; i++) {
        if (strcasecmp(jugadores[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

static int insertarJugadorOrdenado(StatsJugador jugadores[], int *cantidad, const char *nombre, CaraACara caraACara[MAX_JUGADORES][MAX_JUGADORES]) {
    if (*cantidad >= MAX_JUGADORES) {
        return -1;
    }
    int pos = *cantidad;
    for (int i = 0; i < *cantidad; i++) {
        if (strcasecmp(nombre, jugadores[i].nombre) < 0) {
            pos = i;
            break;
        }
    }
    for (int r = *cantidad; r > pos; r--) {
        for (int c = 0; c < *cantidad; c++) {
            caraACara[r][c] = caraACara[r - 1][c];
        }
    }
    for (int r = 0; r <= *cantidad; r++) {
        for (int c = *cantidad; c > pos; c--) {
            caraACara[r][c] = caraACara[r][c - 1];
        }
    }
    for (int i = *cantidad; i > pos; i--) {
        jugadores[i] = jugadores[i - 1];
    }
    strncpy(jugadores[pos].nombre, nombre, MAX_SIZE - 1);
    jugadores[pos].nombre[MAX_SIZE - 1] = '\0';
    jugadores[pos].partidasJugadas = 0;
    jugadores[pos].partidasGanadas = 0;
    jugadores[pos].partidasPerdidas = 0;
    jugadores[pos].empates = 0;
    jugadores[pos].porcentajeVictorias = 0.0f;
    for (int c = 0; c <= *cantidad; c++) {
        caraACara[pos][c].w = 0;
        caraACara[pos][c].l = 0;
        caraACara[pos][c].e = 0;
        caraACara[c][pos].w = 0;
        caraACara[c][pos].l = 0;
        caraACara[c][pos].e = 0;
    }
    (*cantidad)++;
    return pos;
}

int cargarEstadisticas(const char *ruta, StatsJugador jugadores[], int *cantidad, CaraACara caraACara[MAX_JUGADORES][MAX_JUGADORES]) {
    limpiarCaraACara(caraACara);
    *cantidad = 0;
    FILE *fp = fopen(ruta, "r");
    if (!fp) {
        return 0;
    }

    int n = 0;
    if (fscanf(fp, "%d", &n) != 1) {
        fclose(fp);
        return -1;
    }
    if (n < 0) n = 0;
    if (n > MAX_JUGADORES) n = MAX_JUGADORES;

    for (int i = 0; i < n; i++) {
        StatsJugador *j = &jugadores[i];
        if (fscanf(fp, "%99s %d %d %d %d", j->nombre, &j->partidasJugadas, &j->partidasGanadas, &j->partidasPerdidas, &j->empates) != 5) {
            n = i;
            break;
        }
        recalcularPorcentaje(j);
    }
    *cantidad = n;

    char marker[8] = {0};
    fscanf(fp, "%7s", marker);
    if (strcmp(marker, "H2H") == 0) {
        for (int i = 0; i < *cantidad; i++) {
            for (int j = 0; j < *cantidad; j++) {
                if (fscanf(fp, "%d %d %d", &caraACara[i][j].w, &caraACara[i][j].l, &caraACara[i][j].e) != 3) {
                    caraACara[i][j].w = 0;
                    caraACara[i][j].l = 0;
                    caraACara[i][j].e = 0;
                }
            }
        }
    }

    fclose(fp);
    return 0;
}

int guardarEstadisticas(const char *ruta, StatsJugador jugadores[], int cantidad, CaraACara caraACara[MAX_JUGADORES][MAX_JUGADORES]) {
    FILE *fp = fopen(ruta, "w");
    if (!fp) {
        return -1;
    }
    fprintf(fp, "%d\n", cantidad);
    for (int i = 0; i < cantidad; i++) {
        StatsJugador *j = &jugadores[i];
        fprintf(fp, "%s %d %d %d %d\n", j->nombre, j->partidasJugadas, j->partidasGanadas, j->partidasPerdidas, j->empates);
    }
    fprintf(fp, "H2H\n");
    for (int i = 0; i < cantidad; i++) {
        for (int j = 0; j < cantidad; j++) {
            fprintf(fp, "%d %d %d ", caraACara[i][j].w, caraACara[i][j].l, caraACara[i][j].e);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}

int actualizarEstadisticas(const char *ruta, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int estadoPartida, StatsJugador jugadores[], int *cantidad, CaraACara caraACara[MAX_JUGADORES][MAX_JUGADORES]) {
    if (estadoPartida == 0) {
        return 0;
    }
    if (jugador1[0] == '\0' || jugador2[0] == '\0') {
        return 0;
    }

    int idx1 = buscarJugadorPorNombre(jugadores, *cantidad, jugador1);
    if (idx1 < 0) {
        idx1 = insertarJugadorOrdenado(jugadores, cantidad, jugador1, caraACara);
    }
    int idx2 = buscarJugadorPorNombre(jugadores, *cantidad, jugador2);
    if (idx2 < 0) {
        idx2 = insertarJugadorOrdenado(jugadores, cantidad, jugador2, caraACara);
    }

    if (idx1 < 0 || idx2 < 0) {
        return -1;
    }

    jugadores[idx1].partidasJugadas++;
    jugadores[idx2].partidasJugadas++;

    if (estadoPartida == 1) {
        jugadores[idx1].partidasGanadas++;
        jugadores[idx2].partidasPerdidas++;
        if (idx1 != idx2) {
            caraACara[idx1][idx2].w++;
            caraACara[idx2][idx1].l++;
        }
    } else if (estadoPartida == 2) {
        jugadores[idx2].partidasGanadas++;
        jugadores[idx1].partidasPerdidas++;
        if (idx1 != idx2) {
            caraACara[idx2][idx1].w++;
            caraACara[idx1][idx2].l++;
        }
    } else if (estadoPartida == 3) {
        jugadores[idx1].empates++;
        jugadores[idx2].empates++;
        if (idx1 != idx2) {
            caraACara[idx1][idx2].e++;
            caraACara[idx2][idx1].e++;
        }
    }

    recalcularPorcentaje(&jugadores[idx1]);
    recalcularPorcentaje(&jugadores[idx2]);

    guardarEstadisticas(ruta, jugadores, *cantidad, caraACara);
    return 0;
}
//!

bool GuiCircleButton(Vector2 center, float radius, char fichaActual) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointCircle(mouse, center, radius);

    Color color;
    if (fichaActual == 'X') {
        color = RED;
    } else if (fichaActual == 'O') {
        color = GOLD;
    } else{
        color = hovered ? LIGHTGRAY : GRAY;
    }


    DrawCircleV(center, radius, color);

    //? Detectar click
    return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool GuiRectButton(Rectangle rect, const char *text, Sound fx, bool *wasHovered) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, rect);

    
    Color color = hovered ? BORDETARKOV : SINBORDE; //? Elige el color del rectángulo
    Color colorTexto = hovered ? BLACK : AMARILLOTARKOV; //? Elige el color del texto

    
    DrawRectangleRec(rect, color); //? Dibuja el rectángulo con el color previamente seleccionado

    int fontSize = 20;
    int textWidth = MeasureText(text, fontSize);
    //?  Centrar texto dentro del rectángulo
    int textX = rect.x + (rect.width  - textWidth) / 2; //? 
    int textY = rect.y + (rect.height - fontSize) / 2;

    DrawText(text, textX, textY, fontSize, colorTexto);

    //? Sonido del hover
    if (hovered && !(*wasHovered)) {
        PlaySound(fx);
    }

    *wasHovered = hovered;

    //? Detectar click
    return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void drawTurno(Rectangle rect, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int *turno) {
    Color color = (*turno == 0) ? RED : GOLD;

    DrawRectangleRec(rect, color);

    //? Centrar texto dentro del rectangulo
    int fontSize = 18;
    int textWidth = MeasureText(TextFormat("Es el turno de %s", ((*turno == 0) ? jugador1 : jugador2)), fontSize);
    int textX = rect.x + (rect.width - textWidth) / 2;
    int textY = rect.y + (rect.height - fontSize) / 2;

    DrawText(TextFormat("Es el turno de %s", ((*turno == 0) ? jugador1 : jugador2)), textX, textY, fontSize, BLACK);
}

void partidaTerminada(Rectangle rect, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int *estadoPartida) {
    DrawRectangleRec(rect, EXTRACCION);
    int fontSize = 20;
    int textWidth = 0;
    if (*estadoPartida >= 1 && *estadoPartida <= 2) {
        textWidth = MeasureText(TextFormat("%s ha ganado la partida", ((*estadoPartida == 1) ? jugador1 : jugador2)), fontSize);
    } else if (*estadoPartida == 3) {
        textWidth = MeasureText("La partida terminó en empate", fontSize);
    }

    int textX = rect.x + (rect.width - textWidth) / 2;
    int textY = rect.y + (rect.height - fontSize) / 2;

    if (*estadoPartida == 3) {
        DrawText("La partida terminó en empate", textX, textY, fontSize, BLACK);
    } else {
        DrawText(TextFormat("%s ha ganado la partida", ((*estadoPartida == 1) ? jugador1 : jugador2)), textX, textY, fontSize, BLACK);
    }
}

void drawCuadroJugador(char jugador[MAX_SIZE], Texture2D fotoJugador, Rectangle rect, int x) { //? El int x es para controlar la transparencia del cuadro
    Color color = (Color) {0, 0, 0, 252};
    

    Rectangle r1 = {rect.x, rect.y+147, rect.width, rect.height-147};

    Color fondo = {255, 255, 255, x};
    Color amarilloTarkov = {231, 229, 212, x}; //? Se modifica para agregarle transparencia al nombre

    DrawTexture(fotoJugador, rect.x, rect.y, fondo);
    int fontSize = 20;
    int textWidth = MeasureText(jugador, fontSize);
    int textX = r1.x + (r1.width - textWidth) / 2;
    int textY = r1.y + (r1.height - fontSize) / 2;

    DrawText(TextFormat(jugador), textX, textY, fontSize, amarilloTarkov);
}

void drawTablero(int RadioCirculo, char tablero[ROWS][COLS], int *columnaAColocar) {
    DrawRectangle(295, 285, 200, 160, DARKBLUE);
    char fichaActual;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fichaActual = tablero[i][j];
            if (GuiCircleButton((Vector2){317 + (26*j), 300 + (26*i)}, RadioCirculo, fichaActual)) {
                *columnaAColocar = j;
            }
        }
    }
}

void resetearTodo(int *estadoPartida, bool *tableroInicializado, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int *turno, int *modoDeJuego, int *columnaAColocar, int *lleno1, int *lleno2, int *columnasLlenas, bool *editar, bool *editar2, bool *estadisticasActualizadas) {
    *estadoPartida = 0;
    *tableroInicializado = false;
    jugador1[0] = '\0';
    jugador2[0] = '\0';
    *turno = 0;
    *modoDeJuego = 0;
    *columnaAColocar = -1;
    *lleno1 = 0;
    *lleno2 = 0;
    *columnasLlenas = 0;
    *editar = false;
    *editar2 = false;
    *estadisticasActualizadas = false;
}

void cargarImagen(Texture2D *fotoJugador, bool *imagenCargada, char jugador[MAX_SIZE], StatsJugador jugadores[], int *cantidad, int x) {
    if (*imagenCargada) {
        return;
    } else {
        int idJugador = buscarJugadorPorNombre(jugadores, *cantidad, jugador);
        if (x == 1) {
            printf("X = %d\n", x);
            if (jugadores[idJugador].partidasGanadas >= 2 && jugadores[idJugador].partidasGanadas < 4) {
                *fotoJugador = LoadTexture("assets/images/USEC2.png");
            } else if (jugadores[idJugador].partidasGanadas >= 4) {
                *fotoJugador = LoadTexture("assets/images/USEC3.png");
            } else {
                *fotoJugador = LoadTexture("assets/images/USEC1.png");
            }
        } else if (x == 2) {
            if (jugadores[idJugador].partidasGanadas >= 2 && jugadores[idJugador].partidasGanadas < 4) {
                *fotoJugador = LoadTexture("assets/images/BEAR2.png");
            } else if (jugadores[idJugador].partidasGanadas >= 4) {
                *fotoJugador = LoadTexture("assets/images/BEAR3.png");
            } else {
                *fotoJugador = LoadTexture("assets/images/BEAR1.png");
            }
        }
        *imagenCargada = true;
    }
}

void unloadImagen(Texture2D *fotoJugador1, Texture2D *fotoJugador2, bool *imagen1Cargada, bool *imagen2Cargada) {
    UnloadTexture(*fotoJugador1);
    UnloadTexture(*fotoJugador2);
    *imagen1Cargada = false;
    *imagen2Cargada = false;
}

void cargarMusica(Music *musica, int y, bool *musicaCargada) {
    if (*musicaCargada || IsMusicStreamPlaying(*musica)) { //? Si ya hay musica, no se ejecuta el resto
        return;
    }

    if (y) {
        *musica = LoadMusicStream("assets/music/matchSongs/Countdown.mp3");
        PlayMusicStream(*musica);
        *musicaCargada = true;
        return;
    } else {
        int x = generarRandom();
        
        if (x == 1) {
            *musica = LoadMusicStream("assets/music/PrepareForEscape.mp3");
        } else if (x == 2) {
            *musica = LoadMusicStream("assets/music/Desolated.mp3");
        } else if (x == 3) {
            *musica = LoadMusicStream("assets/music/DarkHorizon.mp3");
        } else if (x == 4) {
            *musica = LoadMusicStream("assets/music/TheBloodWeSpill.mp3");
        }
        *musicaCargada = true;
        PlayMusicStream(*musica);
    }
}

void unloadMusica(Music *musica, bool *musicaCargada) {
    UnloadMusicStream(*musica);
    *musicaCargada = false;
}