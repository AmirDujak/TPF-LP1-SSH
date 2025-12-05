# Escape from N-Connect

Juego de 4-en-linea con interfaz hecha en **raylib**/**raygui**. Incluye menus navegables, musica y sonidos, partidas guardadas y un sistema de estadisticas con modo leaderboard y head-to-head.

- **Pantallas**: menu principal, creditos, estadisticas, seleccion de partida (nueva o cargada), partida en curso, menu de pausa y guardado.
- **Modos de juego**: PvP y PvE (IA heuristica simple; prioriza ganar, bloquear y centro).
- **Persistencia**: partidas guardadas en `save1.txt`, `save2.txt`, `save3.txt`; estadisticas en `stats.txt`.
- **Recursos**: `assets/` contiene musica, sonidos y texturas.

## Requisitos
- `gcc` u otro compilador C compatible.
- `raylib` 4.x y dependencias del sistema (GL, pthread, dl, rt, X11 en Linux; adapta las libs segun tu OS).
- El cwd debe contener `assets/` y los archivos `saveX.txt`/`stats.txt`.

## Compilacion rapida
Ejemplo para Linux (mismo comando que `compile.txt`):
```bash
gcc main.c funciones.c funcionesRayLib.c -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```
En macOS/Windows ajusta las banderas segun tu instalacion de raylib.

## Ejecucion
```bash
./main
```
El juego abre una ventana 800x450, reproduce musica (`assets/music/PrepareForEscape.mp3`) y habilita efectos de sonido para hover/click/drag/drop.

## Decisiones de diseño
- Para el diseño se utilizó el videojuego Escape from Tarkov como inspiración principal, tanto como el GUI, efectos de sonido, música e imágenes.
- Se dibuja un rectángulo azul de 200x160 píxeles. Se realizan dos for loops para dibujar los círculos a lo largo del rectángulo azul para simular ser un tablero con huecos.
- Por cada vez que se valida la ubicación a colocar la ficha por un usuario (o la IA), se actualiza la matriz y se cambia el valor del turno por punteros. Si *turno = 0, se cambia a *turno = 1 para que juegue el otro jugador y así sucesivamente. 
- El programa no cuenta con límite de fichas. La partida termina cuando un jugador hace 4 en fila y escapa del N-Connect, o cuando se llena el tablero y ningunó conectó 4 en raya (termina empate).
- Cada círculo dibujado a través de los for loop tienen un índice i y otro índice j. Cuando se clickea el círculo, se pasan los índices a las funciones correspondientes para agregar las fichas al tablero y luego actualizar los colores de los círculos.
- Cada vez que se termina una partida, se guardan los datos del jugador en `stats.txt`. Cuando un jugador tiene 2 o más victorias, se cambia su foto durante la partida. Cuando llega a 4 victorias, la foto cambia a la de un chad de Tarkov. Si el jugador no tiene victorias o tiene menos de 2, su imagen es la de un timmy.
- Las IAs tienen los nombres de Killa, Tagilla, Shturman y Glukhar, quienes serían los bosses de diferentes mapas de Escape from Tarkov. 

## Flujo principal
- **Menu**: botones `Jugar`, `Estadisticas`, `Creditos`, `Salir`.
- **Jugar**: elegir `Partida nueva` o cargar alguno de los 3 slots. Puedes editar nombres de jugadores y elegir modo.
- **Partida**: tablero 6x7. Turnero indica color actual. Menus de pausa permiten volver al menu principal, guardar o salir sin guardar.
- **Estadisticas**: tab `Leaderboard` muestra nombre, jugadas, ganadas, perdidas, empates y win%. Tab `Head-to-Head` permite ver enfrentamientos entre cualquier par.

## Formato de datos
- `stats.txt`: primera linea = cantidad de jugadores. Luego lineas `nombre jugadas ganadas perdidas empates`. Tras la palabra `H2H` se guarda una matriz (max 50x50) de enfrentamientos directos.
- `saveN.txt`: cada slot guarda `nombre1 nombre2 turno modo estadoPartida` seguido de una matriz 6x7 del tablero. Usa `leerInfoGuardado`/`cargarPartida` para leerlos.

## Codigo y entradas utiles
- `main.c`: loop principal de raylib, manejo de pantallas, UI y sonidos.
- `funciones.c`: logica de juego (tablero, IA, reglas), guardados, estadisticas y wrappers GUI (`GuiRectButton`, `GuiCircleButton`, etc.).
- `funciones.h`: constantes, structs (`StatsJugador`, `SaveSlotInfo`, `CaraACara`) y firmas.
- `compile.txt`: comando de referencia para compilar.

## Controles rapidos
- Mouse para menus y para soltar fichas en columnas.
- Botones de pausa: `Menu`, `Volver`, `Guardar`, `Salir sin guardar`.
- La musica se gestiona con `PlayMusicStream`; si necesitas silenciar, ajusta volumenes en `main.c`.

## Mantenimiento rapido
- Al agregar assets, actualiza rutas en `main.c`.
- Si cambias el tamano del tablero, revisa `ROWS/COLS` en `funciones.h` y la logica de guardado/estadisticas.
- Para reseteo completo entre partidas, usar `resetearTodo` (ya limpia tablero, turnos y flags de GUI/estadisticas).

## Dificultades técnicas
- Se intentó agregar 5 músicas diferentes. Cuatro de ellas se usarían para el menú, eligiéndose al azar una cada vez que se vaya al menú. La quinta música sonaría únicamente durante la partida. Al volver al menú principal luego de terminar una partida, el juego crasheaba por errores de la música. Por motivos de tiempo, se terminó descartando la idea de utilizar varias músicas.

## Cracks y creditos
- Autor: Amir Dujak
- Profesores: Vicente Gonzales, Carlos Troya, Martin Monzon y Elias Alvarez
