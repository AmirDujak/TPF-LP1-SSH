#ifndef FUNCIONES_H
#define FUNCIONES_H
#define ROWS 6
#define COLS 7
#define MAX_SIZE 100
#define MAX_JUGADORES 50
#define AMARILLOTARKOV (Color) {231, 229, 212, 255}
#define SINBORDE (Color) {255, 255, 255, 0}
#define BORDETARKOV (Color) {198, 192, 164, 255}
#define EXTRACCION (Color) {157, 219, 35, 255}
#include <stdbool.h>
#include "raylib.h"

typedef struct {
    char nombre[MAX_SIZE];
    int partidasJugadas;
    int partidasGanadas;
    int partidasPerdidas;
    int empates;
    float porcentajeVictorias;
} StatsJugador;

typedef struct {
    int w;
    int l;
    int e;
} CaraACara;

typedef struct {
    bool existe;
    char nombre1[MAX_SIZE];
    char nombre2[MAX_SIZE];
    int turno;
    int modoDeJuego;
    int estadoPartida;
    char tablero[ROWS][COLS];
} SaveSlotInfo;

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
int cargarPartida(char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int *modoDeJuego, int *turno, int *estadoPartida, char tablero[ROWS][COLS], int partidaACargar);
int guardarPartidaEnSlot(const char *ruta, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int modoDeJuego, int turno, int estadoPartida, char tablero[ROWS][COLS]);
int seleccionarColumnaIA(char tablero[ROWS][COLS], int turno);
int cargarEstadisticas(const char *ruta, StatsJugador jugadores[], int *cantidad, CaraACara caraACara[MAX_JUGADORES][MAX_JUGADORES]);
int guardarEstadisticas(const char *ruta, StatsJugador jugadores[], int cantidad, CaraACara caraACara[MAX_JUGADORES][MAX_JUGADORES]);
int actualizarEstadisticas(const char *ruta, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int estadoPartida, StatsJugador jugadores[], int *cantidad, CaraACara caraACara[MAX_JUGADORES][MAX_JUGADORES]);
void asignarNombresIA(int modoDeJuego, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE]);
bool GuiCircleButton(Vector2 center, float radius, char fichaActual);
bool GuiRectButton(Rectangle rect, const char *text, Sound fx, bool *wasHovered);
void drawTurno(Rectangle rect, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int *turno);
void partidaTerminada(Rectangle rect, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int *estadoPartida);
void drawCuadroJugador(char jugador[MAX_SIZE], Texture2D fotoJugador, Rectangle rect, int x); //? El int x es para controlar la transparencia del cuadro
void drawTablero(int RadioCirculo, char tablero[ROWS][COLS], int *columnaAColocar);
void resetearTodo(int *estadoPartida, bool *tableroInicializado, char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int *turno, int *modoDeJuego, int *columnaAColocar, int *lleno1, int *lleno2, int *columnasLlenas, bool *editar, bool *editar2, bool *estadisticasActualizadas);
void cargarImagen(Texture2D *fotoJugador, bool *imagenCargada, char jugador[MAX_SIZE], StatsJugador jugadores[], int *cantidad, int x);
void unloadImagen(Texture2D *fotoJugador1, Texture2D *fotoJugador2, bool *imagen1Cargada, bool *imagen2Cargada);
bool GuiRectButtonGuardado(Rectangle rect, SaveSlotInfo save, Sound fx, bool *wasHovered);
void leerInfoGuardado(const char *ruta, SaveSlotInfo *slot);

#endif
