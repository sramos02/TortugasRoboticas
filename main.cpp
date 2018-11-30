#include "Puntuaciones.h"
#include "Juego.h"
#include "Cartas.h"
#include "checkML.h"
#include "Dibujo.h"
#include <iostream>
using namespace std;

//PROTOTIPOS DE FUNCIONES
int menu();
void ejecutarJuego(tJuego & juego);

//FUNCIONES DEL PROGRAMA

/* Funcion principal del programa, en ella se ejecutan las opciones del menu mostrado al usuario al empezar
 * la partida. Carga inicialmente las puntuaciones y ejecuta el idioma por defecto a español */
int main()
{

    int opc;
    bool abiertoT, abiertoP;
    tPuntuaciones puntos;
    tJuego juego;

    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    setlocale(LC_ALL, "spanish");
    abiertoP = cargarPuntuaciones(puntos);
    opc = menu();

    while (opc != 0){
        if (opc == 1) {
            abiertoT = cargarJuego(juego);
            if (abiertoT) {
                ejecutarJuego(juego);
                actualizarPuntuacion(puntos, juego.jugadores[juego.turnoActual].nombreJugador, juego.numJugadores);
                guardarPuntuaciones(puntos); //Guarda cuando termina de jugar cada partida!
            }
            else { cout << "Lo sentimos, el tablero no pudo abrirse" << endl; }
        }
        else if (abiertoP){ mostrarPuntaciones(puntos); }
        else { cout << "No se ha podido realizar la opcion deseada" << endl; }
        opc = menu();
    }
    liberar(puntos);

    return 0;
}

/* La función menu muestra por pantalla una serie de opciones y pide al usuario una de ellas las veces
* que haga falta para comenzar el juego */
int menu()
{
    int opc;
    cout << endl << "---- MENU ----" << endl;
    cout << "1 - Jugar" << endl;
    cout << "2 - Mostrar puntuaciones" << endl;
    cout << "0 - Salir" << endl;
    cout << "Introduzca una opcion para continuar: ";
    cin >> opc;
    while ((opc < 0) || (opc > 2)) {
        cout << "Error iensperado, introduzca una opci?n valida: ";
        cin.clear();
        cin.ignore();
        cin >> opc;

    }
    return opc;
}

/* Esta función se encarga de comenzar la partida, mostrando la partida una vez cargado el juego y ejecutando
* un buble de juego que continua hasta que alguien gana la partida, finalmente muestra la partida una vez más
* para asegurarse de que el ultimo movimiento ejecutado es recreado en el tablero y mostrarlo a los jugadores */
void ejecutarJuego(tJuego & juego)
{
    bool finPartida = false;
    while (!finPartida) {
        finPartida = ejecutarTurno(juego);
        if (!finPartida) { cambiarTurno(juego); }
    }
    cout << "El juego ha temrinado";
}