#ifndef JUEGO_H
#define JUEGO_H
#include "Cartas.h"
#include <string>
using namespace std;

//****************************************DECLARACION DE CONSTANTES Y TIPOS*******************************************//
const int MAX_JUGADORES = 4;
const int TIPOS_CASILLAS = 6;
const int PALETA[TIPOS_CASILLAS + MAX_JUGADORES] = { 1, 11, 7, 4, 12, 5, 13, 9, 10, 3 };

enum tDir { NORTE, SUR, ESTE, OESTE };
enum tEstadoCasilla { VACIA, HIELO, MURO, CAJA, JOYA, TORTUGA };
enum tTecla { AVANZA, DERECHA, IZQUIERDA, DISPARO, SALIR };
typedef string tNombres[MAX_JUGADORES];

typedef struct {
    int numero;
    tDir direccion;
} tTortuga;

typedef struct {
    tEstadoCasilla estado;
    tTortuga tortuga;
} tCasilla;

typedef struct {
    tMazo mazo;
    tMano mano;
    string nombreJugador;
    tCoordenada coordenada;
    tCoordenada posicionOriginal;
    int colorJugador;
} tJugador;

typedef tCasilla tTablero[NUM_FILAS][NUM_FILAS];
typedef tJugador tJugadores[MAX_JUGADORES];

typedef struct {
    int numJugadores;
    int numJoyas;
    int turnoActual;
    tJugadores jugadores;
    tTablero tablero;
} tJuego;

//**********************************************PROTOTIPOS DE FUNCIONES*************************************************//
//(Separadas segun los distintos bloques)

//**Inicializacion**
void solicitaJugadores(tJuego & juego);
bool cargarJuego(tJuego & juego);

//**Turno**
bool ejecutarTurno(tJuego& juego);
void cambiarTurno(tJuego & juego);

//**Movimiento**
bool movimientoTortuga(tJuego & juego, tCarta carta, int x, int y);
#endif