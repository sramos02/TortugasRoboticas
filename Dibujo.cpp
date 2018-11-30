#include "Dibujo.h"
#include <Windows.h>
#include <iostream>

/*Esta función es la encargada exclusivamente de cambiar el color del fondo, segun el numero de entrada (color)*/
void colorFondo(int color)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, 15 | (color << 4));
}

/* Función encargada de trnasformar el tipo tablero a dibujo y mostrarlo todo por pantalla,
* primero trnasforma los caracteres y luego cambia el color del fondo, tambien encargada de pintar
* la tabla de jugadores con las manos correspondientes */
void mostrarJuego(tJuego & juego)
{
    int jugador;
    system("cls");

    //Representación del tablero
    for (int x = 0; x < NUM_FILAS; x++) {
        for (int y = 0; y < NUM_FILAS; y++) {
            switch (juego.tablero[x][y].estado) {
                case VACIA: colorFondo(PALETA[0]); cout << "  "; break;
                case HIELO: colorFondo(PALETA[1]); cout << "**"; break;
                case MURO: colorFondo(PALETA[2]); cout << "||"; break;
                case CAJA: colorFondo(PALETA[3]); cout << "[]"; break;
                case JOYA: colorFondo(PALETA[4]); cout << "00"; break;
                case TORTUGA: jugador = juego.tablero[x][y].tortuga.numero;
                    colorFondo(juego.jugadores[jugador].colorJugador);
                    if (juego.tablero[x][y].tortuga.direccion == NORTE) { cout << "^^"; }
                    else if (juego.tablero[x][y].tortuga.direccion == SUR) { cout << "vv"; }
                    else if (juego.tablero[x][y].tortuga.direccion == ESTE) { cout << ">>"; }
                    else if (juego.tablero[x][y].tortuga.direccion == OESTE) { cout << "<<"; }
                    break;
            }
        }
        cout << endl;
    }
    cout << endl;

    //Representación de la tabla de jugadores
    colorFondo(0); cout << "TABLA DE JUGADORES:" << endl;
    for (int numJ = 0; numJ < juego.numJugadores; numJ++) {
        colorFondo(juego.jugadores[numJ].colorJugador); cout << numJ << ".   " << juego.jugadores[numJ].nombreJugador;
        colorFondo(0); cout << "\t" << juego.jugadores[numJ].mano.Avanza << " ";
        colorFondo(TIPOS_CASILLAS + MAX_JUGADORES); cout << "^";
        colorFondo(0); cout << juego.jugadores[numJ].mano.Derecha << " ";
        colorFondo(TIPOS_CASILLAS + MAX_JUGADORES); cout << ">";
        colorFondo(0); cout << juego.jugadores[numJ].mano.Izquierda << " ";
        colorFondo(TIPOS_CASILLAS + MAX_JUGADORES); cout << "<";
        colorFondo(0); cout << juego.jugadores[numJ].mano.Laser << " ";
        colorFondo(TIPOS_CASILLAS + MAX_JUGADORES); cout << "~" << endl; colorFondo(0);
    }
}
