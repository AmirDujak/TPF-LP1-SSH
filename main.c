#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "funciones.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


bool GuiCircleButton(Vector2 center, float radius, char fichaActual, const char *text) {
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
    DrawText(text, center.x - MeasureText(text, 20)/2, center.y - 10, 20, BLACK);

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

void drawTablero(int RadioCirculo, char tablero[ROWS][COLS], int *columnaAColocar) {
    DrawRectangle(275, 230, 250, 215, DARKBLUE);
    char fichaActual;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fichaActual = tablero[i][j];
            if (GuiCircleButton((Vector2){295 + (35*j), 250 + (35*i)}, RadioCirculo, fichaActual, "")) {
                *columnaAColocar = j;
            }
        }
    }
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

    //? Cargar la fuente
    Font fuente = LoadFont("assets/font/AGENCYB.ttf");
    


    GameScreen currentScreen = SCREEN_PARTIDA; //! Cambiar a SCREEN_MENU al terminar
    int PosXVolver, PosYVolver, TamXVolver, TamYVolver;

    

    
    Texture2D fondo = LoadTexture("assets/escape_n_connect.png");

    PosXVolver = 680;
    PosYVolver = 20;
    TamXVolver = 100;
    TamYVolver = 40;

    //? Circulos
    int RadioCirculo = 13;
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
            
            drawTablero(RadioCirculo, tablero, &columnaAColocar);
            if (columnaAColocar >= 0) {
                elegirUbicacionDeFicha(&turno, tablero, &estadoPartida, jugador1, jugador2, modoDeJuego, &columnaAColocar);
                PlaySound(sonidoDrop);
            }
            verificarVictoria(tablero, &estadoPartida, &lleno1, &lleno2, &columnasLlenas); //estadoPartida = 1 gano el 1
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