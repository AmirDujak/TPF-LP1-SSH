#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "funciones.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


typedef enum {
    SCREEN_MENU = 0,
    SCREEN_CREDITOS,
    SCREEN_ESTADISTICAS,
    SCREEN_JUGAR, //? Separador
    SCREEN_CONTINUAR_PARTIDA,
    SCREEN_PARTIDA_NUEVA,
    SCREEN_PARTIDAS_GUARDADAS, //? Separador
    SCREEN_PARTIDA,
    SCREEN_PARTIDA_MENU,
    SCREEN_GUARDAR_PARTIDA
} GameScreen;

bool hoverJugar = false;
bool hoverEstadisticas = false;
bool hoverCreditos = false;
bool hoverSalir = false;
bool hoverVolver = false;
bool hoverNuevaPartida = false;
bool hoverPartidasGuardadas = false;
bool hoverEmpezarPartida = false;
bool hoverVolverAlMenu = false;
bool hoverMenu = false;
bool hoverSave1 = false;
bool hoverSave2 = false;
bool hoverSave3 = false;
bool hoverGuardar = false;
bool hoverStatsMenu = false;
bool hoverCreditosMenu = false;
bool hoverSalirSinGuardar = false;
bool save1Leido = false;
bool save2Leido = false;
bool save3Leido = false;
bool tableroInicializado = false;
bool estadisticasActualizadas = false;
bool imagen1Cargada = false;
bool imagen2Cargada = false;

int vistaEstadisticas = 0; //0 leaderboard, 1 head-to-head
int modoH2H = 0; //0 absoluto, 1 porcentual
int scrollLeaderboardY = 0;
int scrollH2HX = 0;
int scrollH2HY = 0;
StatsJugador statsJugadores[MAX_JUGADORES];
CaraACara caraACara[MAX_JUGADORES][MAX_JUGADORES];
int cantidadJugadores = 0;
const char *RUTA_STATS = "stats.txt";
GameScreen previousScreen = SCREEN_MENU;

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
    int estadoPartida = 0;
    int columnaAColocar = -1, filaAColocar;
    int quehacer;
    

    InitWindow(screenWidth, screenHeight, "Escape from N-Connect");
    SetTargetFPS(60);

    InitAudioDevice(); //? Inicia el dispositivo de audio
    Music music = LoadMusicStream("assets/music/PrepareForEscape.mp3");
    SetMasterVolume(1.0f);
    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);

    //? Iniciar los efectos de sonido
    Sound sonidoHover = LoadSound("assets/sounds/hoverSound.ogg");
    Sound sonidoClick = LoadSound("assets/sounds/clickSound.ogg");
    Sound sonidoDrag = LoadSound("assets/sounds/dragSound.ogg"); //! Todavia no se implemento
    Sound sonidoDrop = LoadSound("assets/sounds/dropSound.ogg");
    


    GameScreen currentScreen = SCREEN_MENU;
    int PosXVolver, PosYVolver, TamXVolver, TamYVolver;

    

    
    Texture2D fondo = LoadTexture("assets/escape_n_connect.png");
    Texture2D fotoJugador1;
    Texture2D fotoJugador2;

    cargarEstadisticas(RUTA_STATS, statsJugadores, &cantidadJugadores, caraACara);


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

    SaveSlotInfo save1;
    SaveSlotInfo save2;
    SaveSlotInfo save3;

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
                    previousScreen = SCREEN_MENU;
                    currentScreen = SCREEN_ESTADISTICAS;
                }
                if (GuiRectButton((Rectangle){325, 300, 150, 40}, "Créditos", sonidoHover, &hoverCreditos)) {
                    PlaySound(sonidoClick);
                    previousScreen = SCREEN_MENU;
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
                    currentScreen = previousScreen;
                }
                DrawText("Autor: Amir Dujak\n\nProfesores:\n\nVicente Gonzáles\nCarlos Troya\nMartín Monzon\nElias Álvarez", 310, 100, 20, AMARILLOTARKOV);
                break;

            case SCREEN_ESTADISTICAS: //? Estadísticas
                if (GuiRectButton((Rectangle){PosXVolver, PosYVolver, TamXVolver, TamYVolver}, "Volver", sonidoHover, &hoverVolver)) {
                    PlaySound(sonidoClick);
                    currentScreen = previousScreen;
                }
                DrawText("Estadisticas", 330, 80, 20, AMARILLOTARKOV);
                GuiToggleGroup((Rectangle){250, 120, 220, 32}, "Leaderboard;Head-to-Head", &vistaEstadisticas);

                if (vistaEstadisticas == 0) {
                    DrawText("Nombre", 120, 180, 18, AMARILLOTARKOV);
                    DrawText("Jugadas", 250, 180, 18, AMARILLOTARKOV);
                    DrawText("Ganadas", 360, 180, 18, AMARILLOTARKOV);
                    DrawText("Perdidas", 470, 180, 18, AMARILLOTARKOV);
                    DrawText("Empates", 580, 180, 18, AMARILLOTARKOV);
                    DrawText("Win%", 690, 180, 18, AMARILLOTARKOV);

                    if (cantidadJugadores == 0) {
                        DrawText("Sin partidas registradas", 280, 230, 20, LIGHTGRAY);
                    } else {
                        int indices[MAX_JUGADORES];
                        for (int i = 0; i < cantidadJugadores; i++) indices[i] = i;

                        for (int i = 0; i < cantidadJugadores - 1; i++) {
                            for (int j = i + 1; j < cantidadJugadores; j++) {
                                StatsJugador *a = &statsJugadores[indices[i]];
                                StatsJugador *b = &statsJugadores[indices[j]];
                                bool swap = false;
                                if (b->partidasGanadas > a->partidasGanadas) swap = true;
                                else if (b->partidasGanadas == a->partidasGanadas && b->porcentajeVictorias > a->porcentajeVictorias) swap = true;
                                if (swap) {
                                    int tmp = indices[i];
                                    indices[i] = indices[j];
                                    indices[j] = tmp;
                                }
                            }
                        }

                        int areaX = 110;
                        int areaY = 210;
                        int areaH = 200;
                        int contentH = cantidadJugadores * 26;
                        if (contentH <= areaH) {
                            scrollLeaderboardY = 0;
                        } else {
                            scrollLeaderboardY += (int)(GetMouseWheelMove() * 12);
                            int minScroll = areaH - contentH;
                            if (scrollLeaderboardY < minScroll) scrollLeaderboardY = minScroll;
                            if (scrollLeaderboardY > 0) scrollLeaderboardY = 0;
                        }

                        BeginScissorMode(areaX, areaY, 580, areaH);
                        for (int fila = 0; fila < cantidadJugadores; fila++) {
                            int idx = indices[fila];
                            StatsJugador *j = &statsJugadores[idx];
                            int y = areaY + scrollLeaderboardY + fila * 26;
                            DrawText(j->nombre, 120, y, 16, WHITE);
                            DrawText(TextFormat("%d", j->partidasJugadas), 260, y, 16, WHITE);
                            DrawText(TextFormat("%d", j->partidasGanadas), 370, y, 16, WHITE);
                            DrawText(TextFormat("%d", j->partidasPerdidas), 480, y, 16, WHITE);
                            DrawText(TextFormat("%d", j->empates), 590, y, 16, WHITE);
                            DrawText(TextFormat("%.1f%%", j->porcentajeVictorias), 690, y, 16, WHITE);
                        }
                        EndScissorMode();
                        if (contentH > areaH) {
                            DrawText("Rueda del mouse para scroll", 280, 420, 12, LIGHTGRAY);
                        }
                    }
                } else {
                    DrawText("Head-to-Head", 320, 170, 18, AMARILLOTARKOV);
                    GuiToggleGroup((Rectangle){320, 200, 160, 32}, "Absoluto;%", &modoH2H);

                    if (cantidadJugadores == 0) {
                        DrawText("Sin partidas registradas", 280, 240, 20, LIGHTGRAY);
                        break;
                    }

                    int rowHeaderW = 90;
                    int cellW = 90;
                    int cellH = 26;
                    int areaX = 120;
                    int areaY = 240;
                    int areaW = 620;
                    int areaH = 150;

                    int contentW = rowHeaderW + (cantidadJugadores * cellW);
                    int contentH = cantidadJugadores * cellH;

                    float wheel = GetMouseWheelMove();
                    if (contentH <= areaH) {
                        scrollH2HY = 0;
                    } else {
                        scrollH2HY += (int)(wheel * 12);
                        int minY = areaH - contentH;
                        if (scrollH2HY < minY) scrollH2HY = minY;
                        if (scrollH2HY > 0) scrollH2HY = 0;
                    }

                    if (contentW <= areaW) {
                        scrollH2HX = 0;
                    } else {
                        if (IsKeyDown(KEY_LEFT)) scrollH2HX += 6;
                        if (IsKeyDown(KEY_RIGHT)) scrollH2HX -= 6;
                        int minX = areaW - contentW;
                        if (scrollH2HX < minX) scrollH2HX = minX;
                        if (scrollH2HX > 0) scrollH2HX = 0;
                    }

                    BeginScissorMode(areaX, areaY - 24, areaW, areaH + 30);
                    for (int c = 0; c < cantidadJugadores; c++) {
                        int x = areaX + rowHeaderW + scrollH2HX + cellW * c;
                        DrawText(statsJugadores[c].nombre, x + 6, areaY - 18, 12, AMARILLOTARKOV);
                    }
                    for (int r = 0; r < cantidadJugadores; r++) {
                        int y = areaY + scrollH2HY + cellH * r;
                        DrawText(statsJugadores[r].nombre, areaX + 4, y + 6, 12, AMARILLOTARKOV);
                        for (int c = 0; c < cantidadJugadores; c++) {
                            const CaraACara *celda = &caraACara[r][c];
                            char buffer[32];
                            if (r == c) {
                                strcpy(buffer, "--");
                            } else if (modoH2H == 0) {
                                snprintf(buffer, sizeof(buffer), "%d-%d-%d", celda->w, celda->l, celda->e);
                            } else {
                                int total = celda->w + celda->l + celda->e;
                                if (total == 0) {
                                    strcpy(buffer, "0%-0%-0%");
                                } else {
                                    int pw = (int)((celda->w * 100) / total);
                                    int pl = (int)((celda->l * 100) / total);
                                    int pe = 100 - pw - pl;
                                    snprintf(buffer, sizeof(buffer), "%d%%-%d%%-%d%%", pw, pl, pe);
                                }
                            }
                            int x = areaX + rowHeaderW + scrollH2HX + cellW * c;
                            DrawRectangleLines(x, y, cellW, cellH, BORDETARKOV);
                            DrawText(buffer, x + 8, y + 6, 12, WHITE);
                        }
                    }
                    EndScissorMode();
                    if (contentH > areaH || contentW > areaW) {
                        DrawText("Rueda = scroll vertical, flechas izquierda/derecha = scroll horizontal", 120, 410, 12, LIGHTGRAY);
                    }
                }
                break;


            case SCREEN_JUGAR: //? Partida nueva o partidas guardadas
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
                leerInfoGuardado("save1.txt", &save1);
                leerInfoGuardado("save2.txt", &save2);
                leerInfoGuardado("save3.txt", &save3);
                if (GuiRectButtonGuardado((Rectangle){260, 220, 280, 60}, save1, sonidoHover, &hoverSave1)) {
                    if (save1.existe) {
                        strcpy(jugador1, save1.nombre1);
                        strcpy(jugador2, save1.nombre2);
                        modoDeJuego = save1.modoDeJuego;
                        turno = save1.turno;
                        estadoPartida = save1.estadoPartida;
                        memcpy(tablero, save1.tablero, sizeof(tablero));
                        tableroInicializado = true;
                        estadisticasActualizadas = false;
                        columnaAColocar = -1;
                        currentScreen = SCREEN_PARTIDA;
                    }
                }
                if (GuiRectButtonGuardado((Rectangle){260, 290, 280, 60}, save2, sonidoHover, &hoverSave2)) {
                    if (save2.existe) {
                        strcpy(jugador1, save2.nombre1);
                        strcpy(jugador2, save2.nombre2);
                        modoDeJuego = save2.modoDeJuego;
                        turno = save2.turno;
                        estadoPartida = save2.estadoPartida;
                        memcpy(tablero, save2.tablero, sizeof(tablero));
                        tableroInicializado = true;
                        estadisticasActualizadas = false;
                        columnaAColocar = -1;
                        currentScreen = SCREEN_PARTIDA;
                    }
                }
                if (GuiRectButtonGuardado((Rectangle){260, 360, 280, 60}, save3, sonidoHover, &hoverSave3)) {
                    if (save3.existe) {
                        strcpy(jugador1, save3.nombre1);
                        strcpy(jugador2, save3.nombre2);
                        modoDeJuego = save3.modoDeJuego;
                        turno = save3.turno;
                        estadoPartida = save3.estadoPartida;
                        memcpy(tablero, save3.tablero, sizeof(tablero));
                        tableroInicializado = true;
                        estadisticasActualizadas = false;
                        columnaAColocar = -1;
                        currentScreen = SCREEN_PARTIDA;
                    }
                }
            break;

            case SCREEN_PARTIDA_MENU: // menú dentro de partida
                if (GuiRectButton((Rectangle){PosXVolver, PosYVolver, TamXVolver, TamYVolver}, "Volver", sonidoHover, &hoverVolver)) {
                    PlaySound(sonidoClick);
                    currentScreen = SCREEN_PARTIDA;
                }
                if (GuiRectButton((Rectangle){300, 200, 200, 40}, "Guardar partida", sonidoHover, &hoverGuardar)) {
                    PlaySound(sonidoClick);
                    currentScreen = SCREEN_GUARDAR_PARTIDA;
                }
                if (GuiRectButton((Rectangle){300, 260, 200, 40}, "Ver estadísticas", sonidoHover, &hoverStatsMenu)) {
                    PlaySound(sonidoClick);
                    previousScreen = SCREEN_PARTIDA_MENU;
                    currentScreen = SCREEN_ESTADISTICAS;
                }
                if (GuiRectButton((Rectangle){300, 320, 200, 40}, "Créditos", sonidoHover, &hoverCreditosMenu)) {
                    PlaySound(sonidoClick);
                    previousScreen = SCREEN_PARTIDA_MENU;
                    currentScreen = SCREEN_CREDITOS;
                }
                if (GuiRectButton((Rectangle){300, 380, 200, 40}, "Salir sin guardar", sonidoHover, &hoverSalirSinGuardar)) {
                    PlaySound(sonidoClick);
                    resetearTodo(&estadoPartida, &tableroInicializado, jugador1, jugador2, &turno, &modoDeJuego, &columnaAColocar, &lleno1, &lleno2, &columnasLlenas, &editar, &editar2, &estadisticasActualizadas);
                    unloadImagen(&fotoJugador1, &fotoJugador2, &imagen1Cargada, &imagen2Cargada);
                    currentScreen = SCREEN_MENU;
                }
            break;

            case SCREEN_GUARDAR_PARTIDA: // elegir slot para guardar
                if (GuiRectButton((Rectangle){PosXVolver, PosYVolver, TamXVolver, TamYVolver}, "Volver", sonidoHover, &hoverVolver)) {
                    PlaySound(sonidoClick);
                    currentScreen = SCREEN_PARTIDA_MENU;
                }
                if (GuiRectButton((Rectangle){300, 200, 200, 40}, "Guardar en slot 1", sonidoHover, &hoverSave1)) {
                    PlaySound(sonidoClick);
                    guardarPartidaEnSlot("save1.txt", jugador1, jugador2, modoDeJuego, turno, estadoPartida, tablero);
                    currentScreen = SCREEN_MENU;
                }
                if (GuiRectButton((Rectangle){300, 260, 200, 40}, "Guardar en slot 2", sonidoHover, &hoverSave2)) {
                    PlaySound(sonidoClick);
                    guardarPartidaEnSlot("save2.txt", jugador1, jugador2, modoDeJuego, turno, estadoPartida, tablero);
                    currentScreen = SCREEN_MENU;
                }
                if (GuiRectButton((Rectangle){300, 320, 200, 40}, "Guardar en slot 3", sonidoHover, &hoverSave3)) {
                    PlaySound(sonidoClick);
                    guardarPartidaEnSlot("save3.txt", jugador1, jugador2, modoDeJuego, turno, estadoPartida, tablero);
                    currentScreen = SCREEN_MENU;
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
                if (modoDeJuego == 1 || modoDeJuego == 2) {
                    asignarNombresIA(modoDeJuego, jugador1, jugador2);
                }
                if (imagen1Cargada) {
                    UnloadTexture(fotoJugador1);
                    imagen1Cargada = false;
                }
                if (imagen2Cargada) {
                    UnloadTexture(fotoJugador2);
                    imagen2Cargada = false;
                }
                estadoPartida = 0;
                columnasLlenas = 0;
                lleno1 = 0;
                lleno2 = 0;
                columnaAColocar = -1;
                tableroInicializado = false;
                estadisticasActualizadas = false;
                currentScreen = SCREEN_PARTIDA;
            }
            break;


            case SCREEN_PARTIDA: //? Juego
                cargarImagen(&fotoJugador1, &imagen1Cargada, jugador1, statsJugadores, &cantidadJugadores, 1);
                cargarImagen(&fotoJugador2, &imagen2Cargada, jugador2, statsJugadores, &cantidadJugadores, 2);
                iniciarTablero(tablero, &tableroInicializado);
                if (GuiRectButton((Rectangle){PosXVolver, PosYVolver, TamXVolver, TamYVolver}, "Menú", sonidoHover, &hoverMenu)) {
                    PlaySound(sonidoClick);
                    previousScreen = SCREEN_PARTIDA;
                    currentScreen = SCREEN_PARTIDA_MENU;
                }

                drawTurno((Rectangle){220, 150, 360, 50}, jugador1, jugador2, &turno);

                if (turno == 0) {
                    drawCuadroJugador(jugador1, fotoJugador1, (Rectangle){50, 80, 150, 180}, 255);
                    drawCuadroJugador(jugador2, fotoJugador2, (Rectangle){600, 80, 150, 180}, 150);  
                } else if (turno == 1) {
                    drawCuadroJugador(jugador1, fotoJugador1, (Rectangle){50, 80, 150, 180}, 150);
                    drawCuadroJugador(jugador2, fotoJugador2, (Rectangle){600, 80, 150, 180}, 255);  
                }
                
                drawTablero(RadioCirculo, tablero, &columnaAColocar);

                bool movimientoRealizado = false;
                if (estadoPartida == 0 && (modoDeJuego == 0 || (modoDeJuego == 1 && turno == 0))) {
                    if (columnaAColocar >= 0) {
                        elegirUbicacionDeFicha(&turno, tablero, &estadoPartida, jugador1, jugador2, modoDeJuego, &columnaAColocar);
                        PlaySound(sonidoDrop);
                        movimientoRealizado = true;
                    }
                }

                bool turnoIA = (modoDeJuego == 1 && turno == 1) || (modoDeJuego == 2);
                if (estadoPartida == 0 && !movimientoRealizado && turnoIA) {
                    int columnaIA = seleccionarColumnaIA(tablero, turno);
                    if (columnaIA >= 0) {
                        int columnaTemporal = columnaIA;
                        elegirUbicacionDeFicha(&turno, tablero, &estadoPartida, jugador1, jugador2, modoDeJuego, &columnaTemporal);
                        PlaySound(sonidoDrop);
                    }
                }

                verificarVictoria(tablero, &estadoPartida, &lleno1, &lleno2, &columnasLlenas); //estadoPartida = 1 gano el 1
                if (estadoPartida != 0 && !estadisticasActualizadas) {
                    actualizarEstadisticas(RUTA_STATS, jugador1, jugador2, estadoPartida, statsJugadores, &cantidadJugadores, caraACara);
                    estadisticasActualizadas = true;
                }
                if (estadoPartida) {
                    partidaTerminada((Rectangle){100, 80, 600, 200}, jugador1, jugador2, &estadoPartida);
                    if (GuiRectButton((Rectangle){300, 200, 170, 70}, "Volver al menu", sonidoHover, &hoverVolverAlMenu)) {
                        resetearTodo(&estadoPartida, &tableroInicializado, jugador1, jugador2, &turno, &modoDeJuego, &columnaAColocar, &lleno1, &lleno2, &columnasLlenas, &editar, &editar2, &estadisticasActualizadas);
                        unloadImagen(&fotoJugador1, &fotoJugador2, &imagen1Cargada, &imagen2Cargada);
                        currentScreen = SCREEN_MENU;
                    }
                }
            break;
        }

        EndDrawing();
    }

    UnloadMusicStream(music);
    UnloadSound(sonidoHover);
    UnloadSound(sonidoClick);
    UnloadTexture(fondo);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
