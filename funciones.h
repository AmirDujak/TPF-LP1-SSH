#ifndef FUNCIONES_H
#define FUNCIONES_H
#define ROWS 6
#define COLS 7
#define MAX_SIZE 100
#define AMARILLOTARKOV (Color) {231, 229, 212, 255}
#define SINBORDE (Color) {255, 255, 255, 0}
#define BORDETARKOV (Color) {198, 192, 164, 255}
#define EXTRACCION (Color) {157, 219, 35, 255}
#include <stdbool.h>

// jugador1 jugador2 modoDeJuego turno tablero estadoPartida
void limpiarBuffer(void);
int generarRandom(void);
void leerCadena(char x[MAX_SIZE]);
void reglas(void);
int elegirTurno(void);
void dibujarTurno(int *turno, char X[MAX_SIZE], char Y[MAX_SIZE]);
void iniciarTablero(char b[ROWS][COLS], bool *inicializado);
void menu(int *juego, char X[MAX_SIZE], char Y[MAX_SIZE], int *turno);
void imprimirTablero(char b[ROWS][COLS], int *estado);
int elegirUbicacionDeFicha(int *turno, char matriz[ROWS][COLS], int *estado, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int modoDeJuego, int *columnaAColocar);
void colocarUbicacionIA(int *turno, char matriz[ROWS][COLS], int *estado, int *lleno);
void verificarVictoria(char matriz[ROWS][COLS], int *estado, int *lleno1, int *lleno2, int *lleno);
void guardarPartida(char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int modoDeJuego, int turno, int estadoPartida, char tablero[ROWS][COLS]);
int cargarPartida(char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int *modoDeJuego, int *turno, int *estadoPartida, char tablero[ROWS][COLS]);
int seleccionarColumnaIA(char tablero[ROWS][COLS], int turno);

//? Funciones de RayLib

#endif
