#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "funciones.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


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

    // Detectar click
    return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool GuiRectButton(Rectangle rect, const char *text, Sound fx, bool *wasHovered) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, rect);

    // Elegir color según fichaActual
    Color color = hovered ? BORDETARKOV : SINBORDE;
    Color colorTexto = hovered ? BLACK : AMARILLOTARKOV;

    // Dibujar rectángulo
    DrawRectangleRec(rect, color);


    // Dibujar borde opcional (más lindo visualmente)
    //DrawRectangleLinesEx(rect, 2, DARKGRAY);

    // Centrar texto dentro del rectángulo
    int fontSize = 20;
    int textWidth = MeasureText(text, fontSize);
    int textX = rect.x + (rect.width  - textWidth) / 2;
    int textY = rect.y + (rect.height - fontSize) / 2;

    DrawText(text, textX, textY, fontSize, colorTexto);

    //? Sonido de hover
    if (hovered && !(*wasHovered)) {
        PlaySound(fx);
    }

    *wasHovered = hovered;

    // Detectar click
    return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
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

void partidaTerminada(char jugador1[MAX_SIZE], char jugador2[MAX_SIZE], int *estadoPartida, ) {


}


typedef enum {
    SCREEN_MENU = 0,
    SCREEN_CREDITOS,
    SCREEN_ESTADISTICAS,
    SCREEN_JUGAR, //? Separador
    SCREEN_CONTINUAR_PARTIDA,
    SCREEN_PARTIDA_NUEVA,
    SCREEN_PARTIDAS_GUARDADAS, //? Separador
    SCREEN_PARTIDA
} GameScreen;

typedef struct {
    char *nombre;
    int partidasJugadas;
    int partidasGanadas;
    int partidasPerdidas;
    int empates;
    int porcentajeVictorias;
} Jugador;

bool hoverJugar = false;
bool hoverEstadisticas = false;
bool hoverCreditos = false;;
bool hoverSalir = false;;
bool hoverVolver = false;;
bool hoverNuevaPartida = false;
bool hoverPartidasGuardadas = false;
bool hoverEmpezarPartida = false;

int main(void){         
    int screenWidth = 800;
    int screenHeight = 450;

    Jugador jugadores;
    
    srand(time(NULL)); //Seedea un número random con el tiempo actual de la máquina con la librería de time.h

    int partidaACargar = 0;
    char jugador1[MAX_SIZE] = {'\0'};
    char jugador2[MAX_SIZE] = {'\0'};
    int victoria1 = 0;
    int victoria2 = 0;
    int columnasLlenas = 0;
    int lleno1 = 0;
    int lleno2 = 0; 

    int modoDeJuego = 0;
    int turno = 0;
    char tablero[ROWS][COLS];
    iniciarTablero(tablero);
    int estadoPartida = 0;
    int columnaAColocar = -1, filaAColocar;
    int quehacer;
    

    InitWindow(screenWidth, screenHeight, "Escape from N-Connect");
    SetTargetFPS(60);

    InitAudioDevice(); //? Inicia el dispositivo de audio
    Music music = LoadMusicStream("assets/music/PrepareForEscape.mp3"); //? Carga la musica
    SetMasterVolume(1.0f);
    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);

    //? Iniciar los efectos de sonido
    Sound sonidoHover = LoadSound("assets/sounds/hoverSound.ogg");
    Sound sonidoClick = LoadSound("assets/sounds/clickSound.ogg");
    Sound sonidoDrag = LoadSound("assets/sounds/dragSound.ogg"); //? Todavia no se implemento
    Sound sonidoDrop = LoadSound("assets/sounds/dropSound.ogg");
    


    GameScreen currentScreen = SCREEN_MENU; //! Cambiar a SCREEN_MENU al terminar
    int PosXVolver, PosYVolver, TamXVolver, TamYVolver;

    

    
    Texture2D fondo = LoadTexture("assets/escape_n_connect.png");
    Texture2D fotoJugador1 = LoadTexture("assets/images/USEC3.png");
    Texture2D fotoJugador2 = LoadTexture("assets/images/BEAR3.png");


    PosXVolver = 680;
    PosYVolver = 20;
    TamXVolver = 100;
    TamYVolver = 40;

    //? Circulos
    int RadioCirculo = 8;
    int continuarPartida = 0;
    bool editar = false;
    bool editar2 = false;
    //cargarPartida(jugador1, jugador2, &modoDeJuego, &turno, &estadoPartida, tablero);

    while (!WindowShouldClose()) {
        // Leer tamaño actual de la ventana
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        // Calcular escala manteniendo proporción
        float scale = fmax(
            (float)screenWidth / fondo.width,
            (float)screenHeight / fondo.height
        );

        // Nuevo tamaño escalado
        float newWidth = fondo.width * scale;
        float newHeight = fondo.height * scale;

        // Centrar la imagen (para evitar que aparezca desplazada)
        float offsetX = (screenWidth - newWidth) / 2.0f;
        float offsetY = (screenHeight - newHeight) / 2.0f;

        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureEx( fondo, (Vector2){offsetX, offsetY}, 0.0f, scale, WHITE); //? Se dibuja el fondo

        switch(currentScreen) {
            case SCREEN_MENU: //? Menu principal
            
            if (GuiRectButton((Rectangle){325, 200, 150, 40}, "Jugar", sonidoHover, &hoverJugar)) {
                PlaySound(sonidoClick);
                currentScreen = SCREEN_JUGAR;
            }
            if (GuiRectButton((Rectangle){325, 250, 150, 40}, "Estadísticas", sonidoHover, &hoverEstadisticas)) {
                PlaySound(sonidoClick);
                currentScreen = SCREEN_ESTADISTICAS;
            }
            if (GuiRectButton((Rectangle){325, 300, 150, 40}, "Créditos", sonidoHover, &hoverCreditos)) {
                PlaySound(sonidoClick);
                currentScreen = SCREEN_CREDITOS;
            }
            if (GuiRectButton((Rectangle){325, 350, 150, 40}, "Salir", sonidoHover, &hoverSalir)) {
                PlaySound(sonidoClick);
                return 0;
            }
            break;


            case SCREEN_CREDITOS: //? Créditos
            if (GuiRectButton((Rectangle){PosXVolver, PosYVolver, TamXVolver, TamYVolver}, "Volver", sonidoHover, &hoverVolver)) {
                PlaySound(sonidoClick);
                currentScreen = SCREEN_MENU;
            }
            DrawText("Autor: Amir Dujak\n\nProfesores:\n\nVicente Gonzáles\nCarlos Troya\nMartín Monzon\nElias Álvarez", 310, 100, 20, DARKGRAY);
            break;

            case SCREEN_ESTADISTICAS: //? Estadísticas
            if (GuiRectButton((Rectangle){PosXVolver, PosYVolver, TamXVolver, TamYVolver}, "Volver", sonidoHover, &hoverVolver)) {
                PlaySound(sonidoClick);
                currentScreen = SCREEN_MENU;
            }
            break;


            case SCREEN_JUGAR: //? Continuar partida, Partida nueva o partidas guardadas
            if (GuiRectButton((Rectangle){PosXVolver, PosYVolver, TamXVolver, TamYVolver}, "Volver", sonidoHover, &hoverVolver)) {
                PlaySound(sonidoClick);
                currentScreen = SCREEN_MENU;
            }
            if (GuiRectButton((Rectangle){300, 240, 200, 40}, "Nueva partida", sonidoHover, &hoverNuevaPartida)) {
                PlaySound(sonidoClick);
                currentScreen = SCREEN_PARTIDA_NUEVA;
            }
            if (GuiRectButton((Rectangle){300, 300, 200, 40}, "Partidas Guardadas", sonidoHover, &hoverPartidasGuardadas)) {
                PlaySound(sonidoClick);
                currentScreen = SCREEN_PARTIDAS_GUARDADAS;
            }
            break;


            case SCREEN_PARTIDAS_GUARDADAS: //? Partidas guardadas
            if (GuiRectButton((Rectangle){PosXVolver, PosYVolver, TamXVolver, TamYVolver}, "Volver", sonidoHover, &hoverVolver)) {
                PlaySound(sonidoClick);
                currentScreen = SCREEN_JUGAR;
            }
            break;


            case SCREEN_PARTIDA_NUEVA: //? Crear nueva partida
            if (GuiRectButton((Rectangle){PosXVolver, PosYVolver, TamXVolver, TamYVolver}, "Volver", sonidoHover, &hoverVolver)) {
                PlaySound(sonidoClick);
                currentScreen = SCREEN_JUGAR;
            }
            DrawText("Jugador 1", 88, 104, 20, AMARILLOTARKOV);
            if (GuiTextBox((Rectangle){88, 136, 184, 32}, jugador1, MAX_SIZE, editar)) {
                editar = !editar;
            }
            
            DrawText("Jugador 2", 528, 104, 20, AMARILLOTARKOV);
            if (GuiTextBox((Rectangle){528, 136, 184, 32}, jugador2, MAX_SIZE, editar2)) {
                editar2 = !editar2;
            }

            GuiToggleGroup((Rectangle){302, 200, 64, 38}, "PvP;PvE;EvE", &modoDeJuego); //? PvP sería 0, PvE sería 1, EvE sería 2;
            DrawText("Elige quien empieza la partida", 255, 260, 20, AMARILLOTARKOV);
            GuiToggleGroup((Rectangle){327, 312, 72, 38}, "Jugador 1;Jugador 2", &turno); //? Jugador 1 seria 0, jugador 2 sería 1
            if(GuiRectButton((Rectangle){313, 373, 176, 48}, "Empezar partida", sonidoHover, &hoverEmpezarPartida)) {
                PlaySound(sonidoClick);
                currentScreen = SCREEN_PARTIDA;
            }
            break;


            case SCREEN_PARTIDA: //? Juego
            if (turno == 0) {
                DrawText(TextFormat("Es el turno de %s", jugador1), 100, 100, 20, BLACK);
            } else if (turno == 1) {
                DrawText(TextFormat("Es el turno de %s", jugador2), 100, 100, 20, BLACK);
            }

            if (turno == 0) {
                drawCuadroJugador(jugador1, fotoJugador1, (Rectangle){50, 80, 150, 180}, 255);
                drawCuadroJugador(jugador2, fotoJugador2, (Rectangle){600, 80, 150, 180}, 150);  
            } else if (turno == 1) {
                drawCuadroJugador(jugador1, fotoJugador1, (Rectangle){50, 80, 150, 180}, 150);
                drawCuadroJugador(jugador2, fotoJugador2, (Rectangle){600, 80, 150, 180}, 255);  
            }
            
            drawTablero(RadioCirculo, tablero, &columnaAColocar);
            if (columnaAColocar >= 0) {
                elegirUbicacionDeFicha(&turno, tablero, &estadoPartida, jugador1, jugador2, modoDeJuego, &columnaAColocar);
                PlaySound(sonidoDrop);
            }
            verificarVictoria(tablero, &estadoPartida, &lleno1, &lleno2, &columnasLlenas); //estadoPartida = 1 gano el 1
            if (estadoPartida) {

            }
            break;
        }

        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    UnloadSound(sonidoHover);
    UnloadSound(sonidoClick);
    UnloadTexture(fondo);
    CloseWindow();
    return 0;
}